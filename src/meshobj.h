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

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace OpenPT
{
    struct BoundingBox
    {
        Vector3f vmin, vmax;
        BoundingBox();
        BoundingBox(const Vector3f &vmin_, const Vector3f &vmax_);

        const bool Intersect(const Ray &ray);
    };

    struct Triangle
    {
        BoundingBox bbox;
        std::array<Vector3f, 3> vert;
        std::array<Vector3f, 3> norm;
        std::array<Vector2f, 3> uv;

        Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_);

        const bool Intersect(const Ray &ray, float &t, float &u, float &v);
    };

    class Model : public GeoObj
    {
    public:
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) = 0;
    };

    class Mesh : public Model
    {
    public:
        BoundingBox bbox;
        std::vector<Triangle> faces;

        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) override final;
    };

    class IdealSphere : public Model
    {
    public:
        float radius;
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) override final;
    };
};

#endif // MESHOBJ_H