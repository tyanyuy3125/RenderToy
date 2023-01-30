#ifndef RAY_H
#define RAY_H

#include "rtmath.h"

namespace RenderToy
{
    struct Ray
    {
        Ray() = delete;

        /// @brief Constructor of Ray class.
        /// @param src_ Ray source.
        /// @param normalized_direction_ NORMALIZED ray direction.
        Ray(Vector3f src_, Vector3f normalized_direction_);

        Vector3f src;
        Vector3f direction;

        const Ray operator-(const Ray &ray);
    };

    struct RayState
    {
        float eta;
        Vector3f absorption;
        Vector3f ffnormal;
    };
}

#endif // RAY_H