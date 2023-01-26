#include "surfacepoint.h"
#include "meshobj.h"

#include <cmath>

namespace OpenPT
{
    SurfacePoint::SurfacePoint(const Triangle *triangle_, const Vector3f &position_)
        : triangle(triangle_), position(position_)
    {
    }

    const Vector3f SurfacePoint::GetEmission(const Vector3f &to_pos, const Vector3f &out_dir, const bool is_solid_angle) const
    {
        // emitivity * (out_dir dot lightnorm) * area / distance^2
        const Vector3f ray(to_pos - position);
        const float distance2 = ray.Dot(ray);
        const float cos_area = out_dir.Dot(triangle->NormalC()) * triangle->AreaC();

        const float solidAngle = is_solid_angle ? cos_area / (distance2 >= EPS ? distance2 : 0.0f) : 1.0f;

        return Vector3f(cos_area > 0.0f ? triangle->parent->tex.emitivity * solidAngle : Vector3f::O);
    }

    // const Vector3f SurfacePoint::GetReflection(const Vector3f &in_dir, const Vector3f &in_rad, const Vector3f &out_dir) const
    // {
    //     const float in_dot = in_dir.Dot(triangle->NormalC());
    //     const float out_dot = out_dir.Dot(triangle->NormalC());

    //     return (in_dot < 0.0f) ^ (out_dot < 0.0f) ? Vector3f::O : (in_rad * triangle->parent->tex.kd) * (std::abs(in_dot) / M_PIf32);
    // }

    bool SurfacePoint::GetNextDirection(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o, float &pdf, RayState &state)
    {
        // out_dir = Vector3f::O;

        // float rr = triangle->parent->tex.kd.Dot(Vector3f::White) / 3.0f;

        // if (Random::Float() < rr)
        // {
        //     Vector3f normal(triangle->NormalC());
        //     normal = normal.Dot(in_dir) >= 0.0f ? normal : -normal;
        //     out_dir = triangle->parent->tex.Sample(in_dir, GetNormal());
        // }

        // color_o = GetMaterial().Eval(out_dir, in_dir, GetNormal()) *
        //           Vector3f::Dot(out_dir, GetNormal()) /
        //           (GetMaterial().PDF(out_dir, in_dir, GetNormal()) * rr);

        // return !(out_dir == Vector3f::O);

        auto n = GetNormal();
        auto mat = GetMaterial();

        float roughness = pow(mat.roughness, 2.);

        // sample microfacet normal
        Vector3f t, b;
        basis(n, t, b);
        Vector3f V = toLocal(t, b, n, in_dir);
        Vector3f h = SampleGGXVNDF(V, roughness, roughness, Random::Float(), Random::Float());
        if (h.z < 0.0)
            h = -h;
        h = toWorld(t, b, n, h);

        // fresnel
        float VoH = Vector3f::Dot(in_dir, h);
        Vector3f f0 = Vector3f::Mix(Vector3f(0.04), mat.albedo, mat.metallic);
        Vector3f F = F_Schlick(f0, VoH);
        float dielF = Fresnel(state.lastIOR, mat.ior, std::abs(VoH), 0.0f, 1.0f);

        // lobe weight probability
        float diffW = (1. - mat.metallic) * (1. - mat.spec_trans);
        float reflectW = Luma(F);
        float refractW = (1. - mat.metallic) * (mat.spec_trans) * (1. - dielF);
        float invW = 1. / (diffW + reflectW + refractW);
        diffW *= invW;
        reflectW *= invW;
        refractW *= invW;

        // cdf
        float cdf[3];
        cdf[0] = diffW;
        cdf[1] = cdf[0] + reflectW;
        // cdf[2] = cdf[1] + refractW;

        Vector4f bsdf = Vector4f::O;
        float rnd = Random::Float();
        if (rnd < cdf[0]) // diffuse
        {
            out_dir = cosineSampleHemisphere(n);
            h = (out_dir + in_dir).Normalized();

            float NoL = Vector3f::Dot(n, out_dir);
            float NoV = Vector3f::Dot(n, in_dir);
            if (NoL <= 0.0f || NoV <= 0.0f)
            {
                bsdf = Vector4f::O;
            }
            float LoH = Vector3f::Dot(out_dir, h);
            float pdf = NoL / M_PIf32;

            Vector3f diff = GetMaterial().EvalDisneyDiffuse(NoL, NoV, LoH, roughness) * (Vector3f(1.0f) - F);
            
            bsdf.x = diff.x;
            bsdf.y = diff.y;
            bsdf.z = diff.z;

            bsdf.w = diffW * pdf;
        }
        else if (rnd < cdf[1]) // reflection
        {
            out_dir = Reflect(-in_dir, h);

            float NoL = Vector3f::Dot(n, out_dir);
            float NoV = Vector3f::Dot(n, in_dir);
            if (NoL <= 0.0f || NoV <= 0.0f)
            {
                bsdf = Vector4f::O;
            }
            float NoH = std::min(0.99f, Vector3f::Dot(n, h));
            float pdf = GGXVNDFPdf(NoH, NoV, roughness);

            Vector3f spec = GetMaterial().EvalDisneySpecularReflection(F, NoH, NoV, NoL);
            
            bsdf.x = spec.x;
            bsdf.y = spec.y;
            bsdf.z = spec.z;

            bsdf.w = reflectW * pdf;
        }
        else // refraction
        {
            state.isRefracted = !state.isRefracted;
            float eta = state.lastIOR / mat.ior;
            out_dir = Refract(-in_dir, h, eta);
            state.lastIOR = mat.ior;

            float NoL = Vector3f::Dot(n, out_dir);
            if (NoL <= 0.)
            {
                bsdf = Vector4f::O;
            }
            float NoV = Vector3f::Dot(n, in_dir);
            float NoH = std::min(0.99f, Vector3f::Dot(n, h));
            float LoH = Vector3f::Dot(out_dir, h);

            float pdf;
            Vector3f spec = GetMaterial().EvalDisneySpecularRefraction(dielF, NoH, NoV, NoL, VoH, LoH, eta, pdf);

            bsdf.x = spec.x;
            bsdf.y = spec.y;
            bsdf.z = spec.z;

            bsdf.w = reflectW * pdf;
        }

        bsdf.x *= std::abs(Vector3f::Dot(n, out_dir));
        bsdf.y *= std::abs(Vector3f::Dot(n, out_dir));
        bsdf.z *= std::abs(Vector3f::Dot(n, out_dir));

        color_o = {bsdf.x, bsdf.y, bsdf.z};
        pdf = bsdf.w;

        return !(color_o == Vector3f::O);
    }

    const Triangle *SurfacePoint::GetHitTriangle()
    {
        return triangle;
    }

    const Triangle *SurfacePoint::GetHitTriangle() const
    {
        return triangle;
    }

    const Vector3f SurfacePoint::GetPosition()
    {
        return position;
    }

    const Vector3f SurfacePoint::GetPosition() const
    {
        return position;
    }

    const Mesh *SurfacePoint::GetHitMesh() const
    {
        return triangle->parent;
    }

    const Material &SurfacePoint::GetMaterial() const
    {
        return triangle->parent->tex;
    }

    const Vector3f SurfacePoint::GetNormal() const
    {
        return triangle->NormalC();
    }
}