#include "ray.h"

namespace RenderToy
{
    Ray::Ray(Vector3f src_, Vector3f normalized_direction_) : src(src_), direction(normalized_direction_) {}
    const Ray Ray::operator-(const Ray &ray)
    {
        return Ray(src, -direction);
    }
}