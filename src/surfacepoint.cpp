#include <RenderToy/surfacepoint.h>
#include <RenderToy/object.h>

#include <cmath>

namespace RenderToy
{
    SurfacePoint::SurfacePoint(const Triangle *triangle_, const Vector3f &position_, const float u_, const float v_)
        : triangle(triangle_), position(position_), u(u_), v(v_)
    {
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
        return triangle->NormalC(u, v);
    }
    
    const Vector3f SurfacePoint::GetGeometricalNormal() const
    {
        return triangle->GeometricalNormalC();
    }
}