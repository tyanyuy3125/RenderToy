#include "material.h"
#include "mathfunc.h"
#include "random.h"

#include <cmath>
#include <algorithm>

using namespace RenderToy;

namespace RenderToy
{
    PrincipledBSDF::PrincipledBSDF(const Vector3f &base_color_, const Vector3f &emission_, const float roughness_, const float metallic_, const float spec_trans_, const float ior_, const float anisotropic_, const float subsurface_, const float specular_tint_, const float sheen_, const float sheen_tint_, const float clearcoat_, const float clearcoat_roughness_, const float at_distance_, const Vector3f &extinction_)
        : base_color(base_color_),
          emission(emission_),
          roughness(roughness_),
          metallic(metallic_),
          spec_trans(spec_trans_),
          ior(ior_),
          anisotropic(anisotropic_),
          subsurface(subsurface_),
          specular_tint(specular_tint_),
          sheen(sheen_),
          sheen_tint(sheen_tint_),
          clearcoat(clearcoat_),
          clearcoat_roughness(clearcoat_roughness_),
          at_distance(at_distance_),
          extinction(extinction_)
    {
        if (roughness < 0.001f)
        {
            roughness = 0.001f;
        }
    }

    const float PrincipledBSDF::FresnelMix(const float eta, const float VDotH) const
    {
        float metallic_fresnel = SchlickFresnel(VDotH);
        float dielectric_fresnel = DielectricFresnel(VDotH, eta);
        return Mix(dielectric_fresnel, metallic_fresnel, metallic);
    }

    const Vector3f PrincipledBSDF::EvalDiffuse(const Vector3f Csheen, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const
    {
        pdf = 0.0f;
        if (L.z <= 0.0f)
            return Vector3f::O;

        // Diffuse
        float FL = SchlickFresnel(L.z);
        float FV = SchlickFresnel(V.z);
        float FH = SchlickFresnel(Vector3f::Dot(L, H));
        float Fd90 = 0.5f + 2.0f * Vector3f::Dot(L, H) * Vector3f::Dot(L, H) * roughness;
        float Fd = Mix(1.0f, Fd90, FL) * Mix(1.0f, Fd90, FV);

        // Fake Subsurface TODO: Replace with volumetric scattering
        float Fss90 = Vector3f::Dot(L, H) * Vector3f::Dot(L, H) * roughness;
        float Fss = Mix(1.0f, Fss90, FL) * Mix(1.0f, Fss90, FV);
        float ss = 1.25f * (Fss * (1.0f / (L.z + V.z) - 0.5f) + 0.5f);

        // Sheen
        Vector3f Fsheen = FH * sheen * Csheen;

        pdf = L.z * (1.0f / M_PIf32);
        return ((1.0f / M_PIf32) * Mix(Fd, ss, subsurface) * base_color + Fsheen) * (1.0f - metallic) * (1.0f - spec_trans);
    }

    const Vector3f PrincipledBSDF::EvalSpecReflection(const float eta, const Vector3f specCol, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const
    {
        pdf = 0.0f;
        if (L.z <= 0.0f)
            return Vector3f(0.0f);

        float FM = FresnelMix(eta, Vector3f::Dot(L, H));
        Vector3f F = Mix(specCol, Vector3f(1.0f), Vector3f(FM));
        float D = GTR2(H.z, roughness);
        float G1 = SmithG(std::abs(V.z), roughness);
        float G2 = G1 * SmithG(std::abs(L.z), roughness);
        float jacobian = 1.0f / (4.0f * Vector3f::Dot(V, H));

        pdf = G1 * std::max(0.0f, Vector3f::Dot(V, H)) * D * jacobian / V.z;
        return F * D * G2 / (4.0f * L.z * V.z);
    }

    const Vector3f PrincipledBSDF::EvalSpecRefraction(const float eta, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const
    {
        pdf = 0.0f;
        if (L.z >= 0.0f)
            return Vector3f::O;

        float F = DielectricFresnel(std::abs(Vector3f::Dot(V, H)), eta);
        float D = GTR2(H.z, roughness);
        float denom = Vector3f::Dot(L, H) + Vector3f::Dot(V, H) * eta;
        denom *= denom;
        float G1 = SmithG(std::abs(V.z), roughness);
        float G2 = G1 * SmithG(std::abs(L.z), roughness);
        float jacobian = std::abs(Vector3f::Dot(L, H)) / denom;

        pdf = G1 * std::max(0.0f, Vector3f::Dot(V, H)) * D * jacobian / V.z;

        Vector3f specColor = Vector3f::Pow(base_color, Vector3f(0.5));
        return specColor * (1.0 - metallic) * spec_trans * (1.0 - F) * D * G2 * std::abs(Vector3f::Dot(V, H)) * std::abs(Vector3f::Dot(L, H)) * eta * eta / (denom * std::abs(L.z) * std::abs(V.z));
    }

    const Vector3f PrincipledBSDF::EvalClearcoat(const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const
    {
        pdf = 0.0f;
        if (L.z <= 0.0f)
            return Vector3f::O;

        float FH = DielectricFresnel(Vector3f::Dot(V, H), 1.0f / 1.5f);
        float F = Mix(0.04f, 1.0f, FH);
        float D = GTR1(H.z, clearcoat_roughness);
        float G = SmithG(L.z, 0.25f) * SmithG(V.z, 0.25f);
        float jacobian = 1.0f / (4.0f * Vector3f::Dot(V, H));

        pdf = D * H.z * jacobian;
        return Vector3f(0.25f) * clearcoat * F * D * G / (4.0f * L.z * V.z);
    }

    void PrincipledBSDF::GetSpecColor(const float eta, Vector3f &specCol, Vector3f &sheenCol) const
    {
        float lum = Convert::RGBToLuminance(base_color);
        Vector3f ctint = lum > 0.0f ? base_color / lum : Vector3f(1.0f);
        float F0 = (1.0f - eta) / (1.0f + eta);
        specCol = Mix(F0 * F0 * Mix(Vector3f::White, ctint, Vector3f(specular_tint)), Vector3f(base_color), Vector3f(metallic));
        sheenCol = Mix(Vector3f::White, Vector3f(ctint), Vector3f(sheen_tint));
    }

    void PrincipledBSDF::GetLobeProbabilities(const float eta, const Vector3f specCol, const float approxFresnel, float &diffuseWt, float &specReflectWt, float &specRefractWt, float &clearcoatWt) const
    {
        diffuseWt = Convert::RGBToLuminance(base_color) * (1.0f - metallic) * (1.0f - spec_trans);
        specReflectWt = Convert::RGBToLuminance(Mix(specCol, Vector3f::White, Vector3f(approxFresnel)));
        specRefractWt = (1.0f - approxFresnel) * (1.0f - metallic) * spec_trans * Convert::RGBToLuminance(base_color);
        clearcoatWt = clearcoat * (1.0f - metallic);
        float totalWt = diffuseWt + specReflectWt + specRefractWt + clearcoatWt;

        diffuseWt /= totalWt;
        specReflectWt /= totalWt;
        specRefractWt /= totalWt;
        clearcoatWt /= totalWt;
    }

    const Vector3f PrincipledBSDF::DisneyEval(const RayState state, Vector3f V, Vector3f N, Vector3f L, float &bsdf_pdf) const
    {
        float eta = state.eta;

        bsdf_pdf = 0.0;
        Vector3f f = Vector3f::O;

        Vector3f T, B;
        Onb(N, T, B);
        V = ToLocal(T, B, N, V); // NDotL = L.z; NDotV = V.z; NDotH = H.z
        L = ToLocal(T, B, N, L);

        Vector3f H;
        if (L.z > 0.0)
            H = (L + V).Normalized();
        else
            H = (L + V * eta).Normalized();

        if (H.z < 0.0)
            H = -H;

        // Specular and sheen color
        Vector3f spec_color, sheen_color;
        GetSpecColor(eta, spec_color, sheen_color);

        // Lobe weights
        float diffuse_weight, spec_reflect_weight, spec_refract_weight, clearcoat_weight;
        float fresnel = FresnelMix(eta, Vector3f::Dot(V, H));
        GetLobeProbabilities(eta, spec_color, fresnel, diffuse_weight, spec_reflect_weight, spec_refract_weight, clearcoat_weight);

        float pdf;

        // Diffuse
        if (diffuse_weight > 0.0f && L.z > 0.0f)
        {
            f += EvalDiffuse(sheen_color, V, L, H, pdf);
            bsdf_pdf += pdf * diffuse_weight;
        }

        // Specular Reflection
        if (spec_reflect_weight > 0.0f && L.z > 0.0f && V.z > 0.0f)
        {
            f += EvalSpecReflection(eta, spec_color, V, L, H, pdf);
            bsdf_pdf += pdf * spec_reflect_weight;
        }

        // Specular Refraction
        if (spec_refract_weight > 0.0f && L.z < 0.0f)
        {
            f += EvalSpecRefraction(eta, V, L, H, pdf);
            bsdf_pdf += pdf * spec_refract_weight;
        }

        // Clearcoat
        if (clearcoat_weight > 0.0f && L.z > 0.0f && V.z > 0.0f)
        {
            f += EvalClearcoat(V, L, H, pdf);
            bsdf_pdf += pdf * clearcoat_weight;
        }

        return f * std::abs(L.z);
    }

    const bool PrincipledBSDF::DisneySample(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o, float &pdf, RayState &state) const
    {
        pdf = 0.0f;
        Vector3f f(0.0f);
        auto N = state.ffnormal;

        auto V = in_dir;

        float r1 = Random::Float();
        float r2 = Random::Float();

        Vector3f T, B;
        Onb(N, T, B);
        V = ToLocal(T, B, N, V); // NDotL = L.z; NDotV = V.z; NDotH = H.z

        // Specular and sheen color
        Vector3f specCol, sheenCol;

        float eta = state.eta;
        GetSpecColor(eta, specCol, sheenCol);

        // Lobe weights
        float diffuseWt, specReflectWt, specRefractWt, clearcoatWt;
        // TODO: Recheck fresnel. Not sure if correct. VDotN produces fireflies with rough dielectric.
        // VDotH matches Mitsuba and gets rid of all fireflies but H isn't available at this stage
        float approxFresnel = FresnelMix(eta, V.z);
        GetLobeProbabilities(eta, specCol, approxFresnel, diffuseWt, specReflectWt, specRefractWt, clearcoatWt);

        // CDF for picking a lobe
        float cdf[4];
        cdf[0] = diffuseWt;
        cdf[1] = cdf[0] + specReflectWt;
        cdf[2] = cdf[1] + specRefractWt;
        cdf[3] = cdf[2] + clearcoatWt;

        if (r1 < cdf[0]) // Diffuse Reflection Lobe
        {
            r1 /= cdf[0];
            out_dir = CosineSampleHemisphere(r1, r2);

            Vector3f H = (out_dir + V).Normalized();

            f = EvalDiffuse(sheenCol, V, out_dir, H, pdf);
            pdf *= diffuseWt;
        }
        else if (r1 < cdf[1]) // Specular Reflection Lobe
        {
            r1 = (r1 - cdf[0]) / (cdf[1] - cdf[0]);
            Vector3f H = SampleGGXVNDF(V, roughness, r1, r2);

            if (H.z < 0.0f)
                H = -H;

            out_dir = (Reflect(-V, H)).Normalized();

            f = EvalSpecReflection(eta, specCol, V, out_dir, H, pdf);
            pdf *= specReflectWt;
        }
        else if (r1 < cdf[2]) // Specular Refraction Lobe
        {
            r1 = (r1 - cdf[1]) / (cdf[2] - cdf[1]);
            Vector3f H = SampleGGXVNDF(V, roughness, r1, r2);

            if (H.z < 0.0f)
                H = -H;

            out_dir = Refract(-V, H, eta).Normalized();

            f = EvalSpecRefraction(eta, V, out_dir, H, pdf);
            pdf *= specRefractWt;
        }
        else // Clearcoat Lobe
        {
            r1 = (r1 - cdf[2]) / (1.0 - cdf[2]);
            Vector3f H = SampleGTR1(clearcoat_roughness, r1, r2);

            if (H.z < 0.0f)
                H = -H;

            out_dir = Reflect(-V, H).Normalized();

            f = EvalClearcoat(V, out_dir, H, pdf);
            pdf *= clearcoatWt;
        }

        out_dir = ToWorld(T, B, N, out_dir);
        color_o = f * std::abs(Vector3f::Dot(N, out_dir));

        return !(out_dir == Vector3f::O);
    }
}