#ifndef SURFACEPOINT_H
#define SURFACEPOINT_H

#include "meshobj.h"
#include "mathfunc.h"

namespace OpenPT
{
    class SurfacePoint
    {
    public:
        SurfacePoint( const Triangle *triangle_, const Vector3f& position_);

        const Vector3f GetEmission(const Vector3f &to_pos, const Vector3f &out_dir, const bool is_solid_angle) const;
        const Vector3f GetReflection(const Vector3f &in_dir, const Vector3f &in_rad, const Vector3f &out_dir) const;
        bool GetNextDirection(const Random &random, const Vector3f &in_dir, Vector3f &out_dir, Vector3f &color_o);

        const Triangle *GetHitTriangle();
        const Triangle *GetHitTriangle() const;
        const Vector3f GetPosition();
        const Vector3f GetPosition() const;

    private:
        const Triangle *triangle;
        Vector3f position;
    };
}

#endif // SURFACEPOINT_H