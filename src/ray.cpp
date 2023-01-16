#include "ray.h"

namespace OpenPT
{
    // TODO: assert when not normalized.
    Ray::Ray(Vector3f src_, Vector3f direction_) : src(src_), direction(direction_) {}
}