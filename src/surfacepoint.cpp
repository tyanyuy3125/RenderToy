#include "surfacepoint.h"

#include <cmath>

namespace OpenPT
{
    SurfacePoint::SurfacePoint(const Triangle *triangle_, const Vector3f &position_)
        : triangle(triangle_), position(position_)
    {
    }

    const Vector3f SurfacePoint::GetEmission(const Vector3f &to_pos, const Vector3f &out_dir, const bool is_solid_angle) const
    {
        const Vector3f ray(to_pos - position);
        const float distance2 = ray.Dot(ray);
        const float cos_area = out_dir.Dot(triangle->Normal()) * triangle->Area();

        const float solidAngle = is_solid_angle ? cos_area / (distance2 >= EPS ? distance2 : 0.0f) : 1.0f;

        return Vector3f(cos_area > 0.0f ? triangle->parent->tex.emitivity * solidAngle : Vector3f::O);
    }

    const Vector3f SurfacePoint::GetReflection(const Vector3f &in_dir, const Vector3f &in_rad, const Vector3f &out_dir) const
    {
        const float in_dot = in_dir.Dot(triangle->Normal());
        const float out_dot = out_dir.Dot(triangle->Normal());

        return Vector3f((in_dot < 0.0f) ^ (out_dot < 0.0f) ? Vector3f::O :
                            (in_rad * triangle->parent->tex.reflectivity) * (std::abs(in_dot) / M_PIf32));
    }

    bool SurfacePoint::GetNextDirection(const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o)
    {
        out_dir = Vector3f::O;

        const float reflectivity_mean =
            triangle->parent->tex.reflectivity.Dot(Vector3f::White) / 3.0f;

        if (Random::Float() < reflectivity_mean)
        {
            const float _2pr1 = M_PIf32 * 2.0f * Random::Float();
            const float sr2 = ::sqrt(Random::Float());

            const float x = std::cos(_2pr1) * sr2;
            const float y = std::sin(_2pr1) * sr2;
            const float z = std::sqrt(1.0f - (sr2 * sr2));

            Vector3f normal(triangle->Normal());
            Vector3f tangent(triangle->Tangent());
            normal = normal.Dot(in_dir) >= 0.0f ? normal : -normal;

            out_dir = (tangent * x) + (normal.Cross(tangent) * y) +
                      (normal * z);

            color_o = triangle->parent->tex.reflectivity / reflectivity_mean;
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