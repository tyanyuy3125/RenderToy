/*
 *  OpenPT - Mesh Object Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/21
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MESHOBJ_H
#define MESHOBJ_H

#include "mathfunc.h"
#include "geoobj.h"
#include "ray.h"
#include "bvh.h"
#include "random.h"

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace OpenPT
{
    class Mesh;

    class Triangle
    {
    public:
        const BoundingBox BBox() const;

        std::array<Vector3f, 3> vert;
        std::array<Vector3f, 3> norm;
        std::array<Vector2f, 3> uv;

        Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_);
        /// @brief Do ray-triangle intersection test in WORLD SPACE.
        /// @param ray 
        /// @param t 
        /// @param u 
        /// @param v 
        /// @return 
        const bool Intersect(const Ray &ray, float &t, float &u, float &v) const;
        /// @brief Get sample point in WORLD SPACE.
        /// @param random 
        /// @return 
        const Vector3f GetSamplePoint(const Random &random) const;

        Mesh *parent;
    };

    class Mesh : public GeoObj
    {
    public:
        std::vector<const Triangle *> faces;
    };
};

#endif // MESHOBJ_H