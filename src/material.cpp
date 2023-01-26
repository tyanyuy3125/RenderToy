#include "material.h"
#include "mathfunc.h"
#include "random.h"

#include <cmath>
#include <algorithm>

using namespace OpenPT;

namespace OpenPT
{
    PrincipledBSDF::PrincipledBSDF(const Vector3f &base_color_, const Vector3f &emission_, const float roughness_, const float metallic_, const float anisotropic_, const float subsurface_, const float specular_tint_, const float sheen_, const float sheen_tint_, const float clearcoat_, const float clearcoat_roughness_, const float spec_trans_, const float ior_, const float at_distance_, const Vector3f &extinction_)
        : base_color(base_color_),
          emission(emission_),
          roughness(roughness_),
          metallic(metallic_),
          anisotropic(anisotropic_),
          subsurface(subsurface_),
          specular_tint(specular_tint_),
          sheen(sheen_),
          sheen_tint(sheen_tint_),
          clearcoat(clearcoat_),
          clearcoat_roughness(clearcoat_roughness_),
          spec_trans(spec_trans_),
          ior(ior_),
          at_distance(at_distance_),
          extinction(extinction_)
    {
        if (roughness < 0.001f)
        {
            roughness = 0.001f;
        }
    }

    float PrincipledBSDF::FresnelMix(float eta, float VDotH) const
    {
        float metallicFresnel = SchlickFresnel(VDotH);
        float dielectricFresnel = DielectricFresnel(VDotH, eta);
        return Mix(dielectricFresnel, metallicFresnel, metallic);
    }

    Vector3f PrincipledBSDF::EvalDiffuse(Vector3f Csheen, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
    {
        pdf = 0.0;
        if (L.z <= 0.0)
            return Vector3f(0.0);

        // Diffuse
        float FL = SchlickFresnel(L.z);
        float FV = SchlickFresnel(V.z);
        float FH = SchlickFresnel(Vector3f::Dot(L, H));
        float Fd90 = 0.5 + 2.0 * Vector3f::Dot(L, H) * Vector3f::Dot(L, H) * roughness;
        float Fd = Mix(1.0f, Fd90, FL) * Mix(1.0f, Fd90, FV);

        // Fake Subsurface TODO: Replace with volumetric scattering
        float Fss90 = Vector3f::Dot(L, H) * Vector3f::Dot(L, H) * roughness;
        float Fss = Mix(1.0f, Fss90, FL) * Mix(1.0f, Fss90, FV);
        float ss = 1.25 * (Fss * (1.0 / (L.z + V.z) - 0.5) + 0.5);

        // Sheen
        Vector3f Fsheen = FH * sheen * Csheen;

        pdf = L.z * (1.0f / M_PIf32);
        return ((1.0f / M_PIf32) * Mix(Fd, ss, subsurface) * base_color + Fsheen) * (1.0 - metallic) * (1.0 - spec_trans);
    }

    Vector3f PrincipledBSDF::EvalSpecReflection(float eta, Vector3f specCol, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
    {
        pdf = 0.0;
        if (L.z <= 0.0)
            return Vector3f(0.0);

        float FM = FresnelMix(eta, Vector3f::Dot(L, H));
        Vector3f F = Mix(specCol, Vector3f(1.0), Vector3f(FM));
        float D = GTR2(H.z, roughness);
        float G1 = SmithG(std::abs(V.z), roughness);
        float G2 = G1 * SmithG(std::abs(L.z), roughness);
        float jacobian = 1.0 / (4.0 * Vector3f::Dot(V, H));

        pdf = G1 * std::max(0.0f, Vector3f::Dot(V, H)) * D * jacobian / V.z;
        return F * D * G2 / (4.0f * L.z * V.z);
    }

    Vector3f PrincipledBSDF::EvalSpecRefraction(float eta, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
    {
        pdf = 0.0;
        if (L.z >= 0.0)
            return Vector3f(0.0);

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

    Vector3f PrincipledBSDF::EvalClearcoat(Vector3f V, Vector3f L, Vector3f H, float &pdf) const
    {
        pdf = 0.0;
        if (L.z <= 0.0)
            return Vector3f(0.0);

        float FH = DielectricFresnel(Vector3f::Dot(V, H), 1.0 / 1.5);
        float F = Mix(0.04f, 1.0f, FH);
        float D = GTR1(H.z, clearcoat_roughness);
        float G = SmithG(L.z, 0.25) * SmithG(V.z, 0.25);
        float jacobian = 1.0 / (4.0 * Vector3f::Dot(V, H));

        pdf = D * H.z * jacobian;
        return Vector3f(0.25) * clearcoat * F * D * G / (4.0 * L.z * V.z);
    }

    void PrincipledBSDF::GetSpecColor(const float eta, Vector3f &specCol, Vector3f &sheenCol) const
    {
        float lum = Luminance(base_color);
        Vector3f ctint = lum > 0.0 ? base_color / lum : Vector3f(1.0f);
        float F0 = (1.0 - eta) / (1.0 + eta);
        specCol = Mix(F0 * F0 * Mix(Vector3f(1.0), ctint, Vector3f(specular_tint)), Vector3f(base_color), Vector3f(metallic));
        sheenCol = Mix(Vector3f(1.0), Vector3f(ctint), Vector3f(sheen_tint));
    }

    void PrincipledBSDF::GetLobeProbabilities(float eta, Vector3f specCol, float approxFresnel, float &diffuseWt, float &specReflectWt, float &specRefractWt, float &clearcoatWt) const
    {
        diffuseWt = Luminance(base_color) * (1.0 - metallic) * (1.0 - spec_trans);
        specReflectWt = Luminance(Mix(specCol, Vector3f(1.0), Vector3f(approxFresnel)));
        specRefractWt = (1.0 - approxFresnel) * (1.0 - metallic) * spec_trans * Luminance(base_color);
        clearcoatWt = clearcoat * (1.0 - metallic);
        float totalWt = diffuseWt + specReflectWt + specRefractWt + clearcoatWt;

        diffuseWt /= totalWt;
        specReflectWt /= totalWt;
        specRefractWt /= totalWt;
        clearcoatWt /= totalWt;
    }

    Vector3f PrincipledBSDF::DisneyEval(const RayState state, Vector3f V, Vector3f N, Vector3f L, float &bsdfPdf) const
    {
        float eta = state.lastIOR / ior;

        bsdfPdf = 0.0;
        Vector3f f = Vector3f(0.0);

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
        Vector3f specCol, sheenCol;
        GetSpecColor(eta, specCol, sheenCol);

        // Lobe weights
        float diffuseWt, specReflectWt, specRefractWt, clearcoatWt;
        float fresnel = FresnelMix(eta, Vector3f::Dot(V, H));
        GetLobeProbabilities(eta, specCol, fresnel, diffuseWt, specReflectWt, specRefractWt, clearcoatWt);

        float pdf;

        // Diffuse
        if (diffuseWt > 0.0 && L.z > 0.0)
        {
            f += EvalDiffuse(sheenCol, V, L, H, pdf);
            bsdfPdf += pdf * diffuseWt;
        }

        // Specular Reflection
        if (specReflectWt > 0.0 && L.z > 0.0 && V.z > 0.0)
        {
            f += EvalSpecReflection(eta, specCol, V, L, H, pdf);
            bsdfPdf += pdf * specReflectWt;
        }

        // Specular Refraction
        if (specRefractWt > 0.0 && L.z < 0.0)
        {
            f += EvalSpecRefraction(eta, V, L, H, pdf);
            bsdfPdf += pdf * specRefractWt;
        }

        // Clearcoat
        if (clearcoatWt > 0.0 && L.z > 0.0 && V.z > 0.0)
        {
            f += EvalClearcoat(V, L, H, pdf);
            bsdfPdf += pdf * clearcoatWt;
        }

        return f * std::abs(L.z);
    }
}