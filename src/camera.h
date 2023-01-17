#include "mathfunc.h"
#include "geoobj.h"

namespace OpenPT
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

    static const Camera AcademyCamera = Camera(Matrix4x4f::I, 35.0f, Vector2f(0.825, 0.446), 0.1, 1000);
}