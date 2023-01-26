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

        Vector3f baseColor = Vector3f::White;
        // Vector3f emission;
        float anisotropic = 0.0f;
        float metallic = 0.0f;
        float roughness = 0.0f;
        float subsurface = 0.0f;
        float specularTint = 0.0f;
        float sheen = 0.0f;
        float sheenTint = 0.0f;
        float clearcoat = 0.0f;
        float clearcoatRoughness = 0.0f;
        float specTrans = 0.0f;
        float ior = 1.0f;
        float atDistance = 0.0f;
        Vector3f extinction;

        const float PDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f Sample(const Vector3f &wi, const Vector3f &N) const;

        // private:
        // static const Vector3f EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

        float FresnelMix(float eta, float VDotH) const;

        Vector3f EvalDiffuse(Vector3f Csheen, Vector3f V, Vector3f L, Vector3f H, float &pdf) const;

        Vector3f EvalSpecReflection(float eta, Vector3f specCol, Vector3f V, Vector3f L, Vector3f H, float &pdf) const;

        Vector3f EvalSpecRefraction(float eta, Vector3f V, Vector3f L, Vector3f H, float &pdf) const;

        Vector3f EvalClearcoat(Vector3f V, Vector3f L, Vector3f H, float &pdf) const;

        void GetSpecColor(float eta, Vector3f &specCol, Vector3f &sheenCol) const;

        void GetLobeProbabilities(float eta, Vector3f specCol, float approxFresnel, float &diffuseWt, float &specReflectWt, float &specRefractWt, float &clearcoatWt) const;

        Vector3f DisneyEval(const RayState state, Vector3f V, Vector3f N, Vector3f L, float &bsdfPdf) const;
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