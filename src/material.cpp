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

    Material::Material(const Vector3f &albedo_, const float metalic_, const float roughness_, const Vector3f &emissive_, const float spec_trans_, const float ior_, const float absorption_)
        : albedo(albedo_), metallic(metalic_), roughness(roughness_), emissive(emissive_), spec_trans(spec_trans_), ior(ior_), absorption(absorption_)
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
        switch (type)
        {
        case MaterialType::DIFFUSE:
            float cosalpha = Vector3f::Dot(N, wo);
            if (cosalpha > 0.0f)
            {
                return kd * EvalDiffuse(wi, wo, N);
            }
            else
                return Vector3f(0.0f);
            break;
        }
    }

    const Vector3f Material::Sample(const Vector3f &wi, const Vector3f &N) const
    {
        switch (type)
        {
        case MaterialType::DIFFUSE:
        case MaterialType::MICROFACET_DIFFUSE:
        case MaterialType::MICROFACET_GLOSSY:
            // uniform sample on the hemisphere
            float x_1 = Random::Float(), x_2 = Random::Float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vector3f local_ray(r * std::cos(phi), r * std::sin(phi), z);
            return LocalRayToWorld(local_ray, N);
            break;
        }
    }

    const Vector3f Material::EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N)
    {
        return 1.0f / M_PI;
    }

    const Vector3f Material::EvalDisneyDiffuse(float NoL, float NoV, float LoH, float roughness_) const
    {
        float FD90 = 0.5 + 2. * roughness_ * pow(LoH, 2.);
        float a = F_Schlick(1., FD90, NoL);
        float b = F_Schlick(1., FD90, NoV);

        return albedo * (a * b / M_PIf32);
    }

    const Vector3f Material::EvalDisneySpecularReflection(Vector3f F, float NoH, float NoV, float NoL) const
    {
        float roughness_ = pow(roughness, 2.);
        float D = D_GTR(roughness_, NoH, 2.);
        float G = GeometryTerm(NoL, NoV, pow(0.5 + roughness * .5, 2.));

        Vector3f spec = D * F * G / (4. * NoL * NoV);

        return spec;
    }

    const Vector3f Material::EvalDisneySpecularRefraction(float F, float NoH, float NoV, float NoL, float VoH, float LoH, float eta, float &pdf) const
    {
        float roughness = std::pow(roughness, 2.0f);
        float D = D_GTR(roughness, NoH, 2.0f);
        float G = GeometryTerm(NoL, NoV, std::pow(0.5f + roughness * 0.5f, 2.0f));
        float denom = std::pow(LoH + VoH * eta, 2.0f);

        float jacobian = std::abs(LoH) / denom;
        pdf = SmithG(std::abs(NoL), roughness * roughness) * std::max(0.0f, VoH) * D * jacobian / NoV;

        Vector3f spec = Vector3f::Pow(Vector3f(1.0f) - albedo, Vector3f(0.5f)) * D * (1.0f - F) * G * std::abs(VoH) * jacobian * std::pow(eta, 2.0f) / std::abs(NoL * NoV);
        return spec;
    }
}