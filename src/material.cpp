#include "material.h"
#include "mathfunc.h"
#include "random.h"

#include <cmath>
#include <algorithm>

using namespace OpenPT;

namespace OpenPT
{

    OpenPT::Material::Material()
        : type(MaterialType::DIFFUSE), kd(Vector3f::O), emitivity(Vector3f::O)
    {
    }

    Material::Material(MaterialType type_, const Vector3f &reflectivity_, const Vector3f &emitivity_)
        : type(type_), kd(reflectivity_), emitivity(emitivity_)
    {
    }

    const float Material::PDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const
    {
        switch (type)
        {
        case MaterialType::DIFFUSE:
        case MaterialType::MICROFACET_DIFFUSE:
        case MaterialType::MICROFACET_GLOSSY:
            // uniform sample probability 1 / (2 * PI)
            if (Vector3f::Dot(wo, N) > 0.0f)
                return 1.0f / M_PI;
            else
                return 0.0f;
            break;
        }
    }

    const Vector3f Material::Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const
    {
        // switch (type)
        // {
        // case MaterialType::DIFFUSE:
        //     float cosalpha = Vector3f::Dot(N, wo);
        //     if (cosalpha > 0.0f)
        //     {
        //         return kd * EvalDiffuse(wi, wo, N);
        //     }
        //     else
        //         return Vector3f(0.0f);
        //     break;
        // }
    }

    const Vector3f Material::Sample(const Vector3f &wi, const Vector3f &N) const
    {
        // switch (type)
        // {
        // case MaterialType::DIFFUSE:
        // case MaterialType::MICROFACET_DIFFUSE:
        // case MaterialType::MICROFACET_GLOSSY:
        //     // uniform sample on the hemisphere
        //     float x_1 = Random::Float(), x_2 = Random::Float();
        //     float z = std::fabs(1.0f - 2.0f * x_1);
        //     float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
        //     Vector3f local_ray(r * std::cos(phi), r * std::sin(phi), z);
        //     return LocalRayToWorld(local_ray, N);
        //     break;
        // }
        return Vector3f::O;
    }

    // const Vector3f Material::EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N)
    // {
    //     return 1.0f / M_PI;
    // }

    float Material::FresnelMix(float eta, float VDotH) const
    {
        float metallicFresnel = SchlickFresnel(VDotH);
        float dielectricFresnel = DielectricFresnel(VDotH, eta);
        return Mix(dielectricFresnel, metallicFresnel, metallic);
    }

    Vector3f Material::EvalDiffuse(Vector3f Csheen, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
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
        return ((1.0f / M_PIf32) * Mix(Fd, ss, subsurface) * baseColor + Fsheen) * (1.0 - metallic) * (1.0 - specTrans);
    }

    Vector3f Material::EvalSpecReflection(float eta, Vector3f specCol, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
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

    Vector3f Material::EvalSpecRefraction(float eta, Vector3f V, Vector3f L, Vector3f H, float &pdf) const
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

        Vector3f specColor = Vector3f::Pow(baseColor, Vector3f(0.5));
        return specColor * (1.0 - metallic) * specTrans * (1.0 - F) * D * G2 * std::abs(Vector3f::Dot(V, H)) * std::abs(Vector3f::Dot(L, H)) * eta * eta / (denom * std::abs(L.z) * std::abs(V.z));
    }

    Vector3f Material::EvalClearcoat(Vector3f V, Vector3f L, Vector3f H, float &pdf) const
    {
        pdf = 0.0;
        if (L.z <= 0.0)
            return Vector3f(0.0);

        float FH = DielectricFresnel(Vector3f::Dot(V, H), 1.0 / 1.5);
        float F = Mix(0.04f, 1.0f, FH);
        float D = GTR1(H.z, clearcoatRoughness);
        float G = SmithG(L.z, 0.25) * SmithG(V.z, 0.25);
        float jacobian = 1.0 / (4.0 * Vector3f::Dot(V, H));

        pdf = D * H.z * jacobian;
        return Vector3f(0.25) * clearcoat * F * D * G / (4.0 * L.z * V.z);
    }

    void Material::GetSpecColor(const float eta, Vector3f &specCol, Vector3f &sheenCol) const
    {
        float lum = Luminance(baseColor);
        Vector3f ctint = lum > 0.0 ? baseColor / lum : Vector3f(1.0f);
        float F0 = (1.0 - eta) / (1.0 + eta);
        specCol = Mix(F0 * F0 * Mix(Vector3f(1.0), ctint, Vector3f(specularTint)), Vector3f(baseColor), Vector3f(metallic));
        sheenCol = Mix(Vector3f(1.0), Vector3f(ctint), Vector3f(sheenTint));
    }

    void Material::GetLobeProbabilities(float eta, Vector3f specCol, float approxFresnel, float &diffuseWt, float &specReflectWt, float &specRefractWt, float &clearcoatWt) const
    {
        diffuseWt = Luminance(baseColor) * (1.0 - metallic) * (1.0 - specTrans);
        specReflectWt = Luminance(Mix(specCol, Vector3f(1.0), Vector3f(approxFresnel)));
        specRefractWt = (1.0 - approxFresnel) * (1.0 - metallic) * specTrans * Luminance(baseColor);
        clearcoatWt = clearcoat * (1.0 - metallic);
        float totalWt = diffuseWt + specReflectWt + specRefractWt + clearcoatWt;

        diffuseWt /= totalWt;
        specReflectWt /= totalWt;
        specRefractWt /= totalWt;
        clearcoatWt /= totalWt;
    }

    Vector3f Material::DisneyEval(const RayState state, Vector3f V, Vector3f N, Vector3f L, float &bsdfPdf) const
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