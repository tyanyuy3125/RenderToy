#include "material.h"
#include "mathfunc.h"
#include "random.h"

#include <cmath>
#include <algorithm>

using namespace OpenPT;

const Vector3f LocalRayToWorld(const Vector3f &a, const Vector3f &N)
{
    Vector3f B, C;
    if (std::fabs(N.x) > std::fabs(N.y))
    {
        float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
        C = Vector3f(N.z * invLen, 0.0f, -N.x * invLen);
    }
    else
    {
        float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
        C = Vector3f(0.0f, N.z * invLen, -N.y * invLen);
    }
    B = Vector3f::Cross(C, N);
    return a.x * B + a.y * C + a.z * N;
}

void Fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr)
{
    float cosi = std::clamp(Vector3f::Dot(I, N), -1.0f, 1.0f);
    float etai = 1, etat = ior;
    if (cosi > 0)
    {
        std::swap(etai, etat);
    }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1)
    {
        kr = 1;
    }
    else
    {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}

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
}