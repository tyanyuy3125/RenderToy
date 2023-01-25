#include "texture.h"
#include "mathfunc.h"

#include <cmath>
#include <algorithm>

using namespace OpenPT;

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
        case MaterialType::DIFFUSE: // case MICROFACET_DIFFUSE: case MICROFACET_GLOSSY:
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

    const Vector3f Material::EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N)
    {
        return 1.0f / M_PI;
    }
}