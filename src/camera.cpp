#include "camera.h"

namespace RenderToy
{
    Camera::Camera(Matrix4x4f object_to_world_, float focal_length_, Vector2f gate_dimension_, float near_clipping_plane_, float far_clipping_plane_)
        : focal_length(focal_length_),
          gate_dimension(gate_dimension_),
          near_clipping_plane(near_clipping_plane_),
          far_clipping_plane(far_clipping_plane_)
    {
        GeoObj::SetO2W(object_to_world);
    }
}