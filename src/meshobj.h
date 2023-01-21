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

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace OpenPT
{
    class Model : public GeoObj
    {
    public:
        BoundingBox bbox;
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) const = 0;
    };

    class Triangle : public Model
    {
    public:
        std::array<Vector3f, 3> vert;
        std::array<Vector3f, 3> norm;
        std::array<Vector2f, 3> uv;

        Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_);

        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) const override final;
    };

    class Mesh : public Model
    {
    public:
        BVH *bvh{nullptr};
        std::vector<const Model *> faces;

        void BuildBVH();
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) const override final;
    };

    class IdealSphere : public Model
    {
    public:
        float radius;
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) const override final;
    };
};

#endif // MESHOBJ_H