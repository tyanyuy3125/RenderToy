#ifndef CAMERA_H
#define CAMERA_H

#include "mathfunc.h"
#include "geoobj.h"

namespace RenderToy
{
    class Camera : public GeoObj
    {
    public:
        float focal_length; // in mm
        Vector2f gate_dimension; // in inches
        float near_clipping_plane;
        float far_clipping_plane;

        Camera(Matrix4x4f object_to_world_, 
                float focal_length_,
                Vector2f gate_dimension_,
                float near_clipping_plane_,
                float far_clipping_plane_);
    };

    static const Camera academy_camera = Camera(Matrix4x4f::I, 35.0f, Vector2f(0.825f, 0.446f), 0.1f, 1000.0f);
}

#endif // CAMERA_H