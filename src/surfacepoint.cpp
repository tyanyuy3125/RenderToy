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

    bool SurfacePoint::GetNextDirection(const Vector3f &in_dir, Vector3f &out_dir, float &rr)
    // bool SurfacePoint::GetNextDirection(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o)
    {
        out_dir = Vector3f::O;

        rr = triangle->parent->tex.kd.Dot(Vector3f::White) / 3.0f;

        if (Random::Float() < rr)
        {
            Vector3f normal(triangle->NormalC());
            normal = normal.Dot(in_dir) >= 0.0f ? normal : -normal;
            out_dir = triangle->parent->tex.Sample(in_dir, triangle->NormalC());
        }

        return !(out_dir == Vector3f::O);
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
}