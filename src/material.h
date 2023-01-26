#ifndef MATERIAL_H
#define MATERIAL_H

#include "mathfunc.h"

namespace OpenPT
{
    struct PrincipledBSDF
    {
        Vector3f base_color = Vector3f::White;
        Vector3f emission = Vector3f::O;
        float roughness = 0.5f;
        float metallic = 0.0f;
        float anisotropic = 0.0f;
        float subsurface = 0.0f;
        float specular_tint = 0.0f;
        float sheen = 0.0f;
        float sheen_tint = 0.5f;
        float clearcoat = 0.0f;
        float clearcoat_roughness = 0.03f;
        float spec_trans = 0.0f;
        float ior = 1.45f;
        float at_distance = 1.0f;
        Vector3f extinction = Vector3f::White;

        PrincipledBSDF() = default;
        /// @brief Construct PrincipledBSDF. Explanations on parameters are copied from Principled BSDF section of Blender Manual.
        /// @param base_color_ Diffuse or metal surface color.
        /// @param emission_ Light emission from the surface.
        /// @param roughness_ Specifies microfacet roughness of the surface for diffuse and specular reflection.
        /// @param metallic_ Blends between a non-metallic and metallic material model
        /// @param anisotropic_ Amount of anisotropy for specular reflection.
        /// @param subsurface_ Mix between diffuse and subsurface scattering.
        /// @param specular_tint_ Tints the facing specular reflection using the base color, while glancing reflection remains white.
        /// @param sheen_ Amount of soft velvet like reflection near edges, for simulating materials such as cloth.
        /// @param sheen_tint_ Mix between white and using base color for sheen reflection.
        /// @param clearcoat_ Extra white specular layer on top of others.
        /// @param clearcoat_roughness_ Roughness of clearcoat specular.
        /// @param spec_trans_
        /// @param ior_ Index of refraction for transmission.
        /// @param at_distance_
        /// @param extinction_
        PrincipledBSDF(const Vector3f &base_color_,
                       const Vector3f &emission_ = Vector3f::O,
                       const float roughness_ = 0.5f,
                       const float metallic_ = 0.0f,
                       const float anisotropic_ = 0.0f,
                       const float subsurface_ = 0.0f,
                       const float specular_tint_ = 0.0f,
                       const float sheen_ = 0.0f,
                       const float sheen_tint_ = 0.5f,
                       const float clearcoat_ = 0.0f,
                       const float clearcoat_roughness_ = 0.03f,
                       const float spec_trans_ = 0.0f,
                       const float ior_ = 1.00f,
                       const float at_distance_ = 1.0f,
                       const Vector3f &extinction_ = Vector3f::White);

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