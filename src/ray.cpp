#include "ray.h"

namespace OpenPT
{
    Ray::Ray(Vector3f src_, Vector3f normalized_direction_) : src(src_), direction(normalized_direction_) {}
}