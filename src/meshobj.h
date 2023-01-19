/*
 *  OpenPT - Mesh Object Module
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
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
    class Model : public GeoObj
    {
    public:
        virtual const bool Intersect(const Ray &ray, float &t, float &u, float &v) = 0;
    };

    class Mesh : public Model
    {
    public:
        std::vector<Vector3f> geo_vert;
        std::vector<Vector3f> vert_norm;
        std::vector<Vector2f> vert_tex;

        // Currently, we only support triangulated mesh!
        // TODO <tianyu@illumiart.net>: Too ugly! Needs refactor.
        std::vector<std::array<std::array<int, 3>, 3>> face;

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