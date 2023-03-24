#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtmath.h"
#include "ray.h"

#include <string>

namespace RenderToy
{
    /// @brief Disney Principled BSDF
    struct PrincipledBSDF
    {
        std::string name = "unnamed";

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

        /// @brief Construct a default Principled BSDF object.
        PrincipledBSDF() = default;
        /// @brief Construct PrincipledBSDF. Explanations on parameters are copied from Principled BSDF section of Blender Manual.
        /// @param base_color_ Diffuse or metal surface color.
        /// @param emission_ Light emission from the surface.
        /// @param roughness_ Specifies microfacet roughness of the surface for diffuse and specular reflection.
        /// @param metallic_ Blends between a non-metallic and metallic material model
        /// @param spec_trans_ Mix between fully opaque surface at zero and fully glass like transmission at one.
        /// @param ior_ Index of refraction for transmission.
        /// @param anisotropic_ Amount of anisotropy for specular reflection.
        /// @param subsurface_ Mix between diffuse and subsurface scattering.
        /// @param specular_tint_ Tints the facing specular reflection using the base color, while glancing reflection remains white.
        /// @param sheen_ Amount of soft velvet like reflection near edges, for simulating materials such as cloth.
        /// @param sheen_tint_ Mix between white and using base color for sheen reflection.
        /// @param clearcoat_ Extra white specular layer on top of others.
        /// @param clearcoat_roughness_ Roughness of clearcoat specular.
        /// @param at_distance_
        /// @param extinction_
        PrincipledBSDF(const Vector3f &base_color_,
                       const Vector3f &emission_ = Vector3f::O,
                       const float roughness_ = 0.5f,
                       const float metallic_ = 0.0f,
                       const float spec_trans_ = 0.0f,
                       const float ior_ = 1.45f,
                       const float anisotropic_ = 0.0f,
                       const float subsurface_ = 0.0f,
                       const float specular_tint_ = 0.0f,
                       const float sheen_ = 0.0f,
                       const float sheen_tint_ = 0.0f,
                       const float clearcoat_ = 0.0f,
                       const float clearcoat_roughness_ = 0.0f,
                       const float at_distance_ = 1.0f,
                       const Vector3f &extinction_ = Vector3f::White);

        const Vector3f Eval(const RayState state, Vector3f V, Vector3f N, Vector3f L, float &bsdf_pdf) const;
        const bool Sample(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o, float &pdf, RayState &state) const;

    private:
        const float FresnelMix(const float eta, const float VDotH) const;
        const Vector3f EvalDiffuse(const Vector3f Csheen, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const;
        const Vector3f EvalSpecReflection(const float eta, const Vector3f specCol, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const;
        const Vector3f EvalSpecRefraction(const float eta, const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const;
        const Vector3f EvalClearcoat(const Vector3f V, const Vector3f L, const Vector3f H, float &pdf) const;
        void GetSpecColor(const float eta, Vector3f &specCol, Vector3f &sheenCol) const;
        void GetLobeProbabilities(const float eta, const Vector3f specCol, const float approxFresnel, float &diffuseWt, float &specReflectWt, float &specRefractWt, float &clearcoatWt) const;
    };

    struct EmissiveBSDF : public PrincipledBSDF
    {
        EmissiveBSDF(const Vector3f &emission_, const float strength_ = 1.0f);
    };

    struct DiffuseBSDF : public PrincipledBSDF
    {
        DiffuseBSDF(const Vector3f &base_color_, const float roughness_ = 0.5f);
    };

    struct GlassBSDF : public PrincipledBSDF
    {
        GlassBSDF(const Vector3f &base_color_, const float roughness_ = 0.5f, const float ior_ = 1.45f);
    };

    struct GlossyBSDF : public PrincipledBSDF
    {
        GlossyBSDF(const Vector3f &base_color_, const float roughness_ = 0.5f);
    };

    struct ITexture
    {
        virtual void Sample(const float u, const float v) const = 0;
    };
}

#endif // MATERIAL_H