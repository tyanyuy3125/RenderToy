#include "surfacepoint.h"
#include "object.h"

#include <cmath>

namespace RenderToy
{
    SurfacePoint::SurfacePoint(const Triangle *triangle_, const Vector3f &position_)
        : triangle(triangle_), position(position_)
    {
    }

    const Vector3f SurfacePoint::GetEmission(const Vector3f &to_pos, const Vector3f &out_dir, const bool is_solid_angle, float &pdf) const
    {
        /*             current point
        emitter    ----*----
                        \
                         \
                          \ out_dir
                           \
                            v
                         ----*----     receiver
                              to_pos
        */
        const Vector3f ray(to_pos - position);
        const float distance2 = ray.Dot(ray);
        const float cos_area = out_dir.Dot(triangle->NormalC()) * triangle->AreaC();
        const float solid_angle = is_solid_angle ? cos_area / (distance2 >= kFloatEpsilon ? distance2 : kFloatEpsilon) : 1.0f / (distance2 >= kFloatEpsilon ? distance2 : kFloatEpsilon);
        pdf = std::abs(1.0f / solid_angle); // TODO: Optimize. abs for culling.
        return triangle->parent->tex->emission;
    }

    const Triangle *SurfacePoint::GetHitTriangle()
    {
        return triangle;
    }

    const Triangle *SurfacePoint::GetHitTriangle() const
    {
        return triangle;
    }

    Vector3f &SurfacePoint::GetPosition()
    {
        return position;
    }

    const Vector3f &SurfacePoint::GetPosition() const
    {
        return position;
    }

    const Mesh *SurfacePoint::GetHitMesh() const
    {
        return triangle->parent;
    }

    const PrincipledBSDF *SurfacePoint::GetMaterial() const
    {
        return triangle->parent->tex;
    }

    const Vector3f SurfacePoint::GetNormal() const
    {
        return triangle->NormalC();
    }
}