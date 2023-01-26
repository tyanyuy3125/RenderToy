#ifndef MATERIAL_H
#define MATERIAL_H

#include "mathfunc.h"

namespace OpenPT
{
    enum class MaterialType
    {
        DIFFUSE,
        MICROFACET_DIFFUSE,
        MICROFACET_GLOSSY
    };

    struct Material
    {
        MaterialType type;
        Vector3f kd;
        Vector3f emitivity;
        Material();
        Material(MaterialType type_, const Vector3f &kd_, const Vector3f &emitivity_);

        Vector3f albedo = Vector3f::White;
        float metallic = 0.0f;
        float roughness = 0.1f;
        Vector3f emissive;
        float spec_trans = 0.0f;
        float ior = 1.5f;
        float absorption = 0.0f;
        Material(const Vector3f &albedo_,
                 const float metalic_,
                 const float roughness_,
                 const Vector3f &emissive_,
                 const float spec_trans_,
                 const float ior_,
                 const float absorption_);

        const float PDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f Sample(const Vector3f &wi, const Vector3f &N) const;

    // private:
        static const Vector3f EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

        const Vector3f EvalDisneyDiffuse(float NoL, float NoV, float LoH, float roughness) const;
        const Vector3f EvalDisneySpecularReflection(Vector3f F, float NoH, float NoV, float NoL) const;
        const Vector3f EvalDisneySpecularRefraction(float F, float NoH, float NoV, float NoL, float VoH, float LoH, float eta, float &pdf) const;
    };

    struct ITexture
    {
        virtual void Sample(const float u, const float v) const = 0;
    };
}

namespace OpenPT::ProceduralTexture
{
    struct PerlinNoiseTexture : public ITexture
    {
        virtual void Sample(const float u, const float v) const override final;
    };

    struct CheckboardTexture : public ITexture
    {
        virtual void Sample(const float u, const float v) const override final;
    };
}

#endif // MATERIAL_H