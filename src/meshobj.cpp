#include "meshobj.h"

#include <limits>
#include <cmath>

namespace OpenPT
{
    const BoundingBox Triangle::BBox() const
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];
        v0 = parent->O2WTransform(v0);
        v1 = parent->O2WTransform(v1);
        v2 = parent->O2WTransform(v2);

        // Vector3f vertexs[3] = {v0, v1, v2};
        // float bound[6];

        Vector3f bmax = {
            std::max(v0.x, std::max(v1.x, v2.x)),
            std::max(v0.y, std::max(v1.y, v2.y)),
            std::max(v0.z, std::max(v1.z, v2.z))};
        Vector3f bmin = {
            std::min(v0.x, std::min(v1.x, v2.x)),
            std::min(v0.y, std::min(v1.y, v2.y)),
            std::min(v0.z, std::min(v1.z, v2.z))};

        return BoundingBox(bmin, bmax);
        // // initialize
        // for (int i = 6; i-- > 0; bound[i] = vertexs[2][i % 3])
        // {
        // }

        // // expand
        // for (int i = 0; i < 3; ++i)
        // {
        //     for (int j = 0, d = 0, m = 0; j < 6; ++j, d = j / 3, m = j % 3)
        //     {
        //         // include some tolerance
        //         const float v = vertexs[i][m] + ((d ? 1.0 : -1.0) * 1.0f / 1024.0f);
        //         bound[j] = (bound[j] > v) ^ d ? v : bound[j];
        //     }
        // }

        // return BoundingBox({bound[0], bound[1], bound[2]},{bound[3], bound[4], bound[5]});
    }

    Triangle::Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_)
        : vert(vert_), norm(norm_), uv(uv_), parent(parent_)
    {
    }

    const bool Triangle::Intersect(const Ray &ray, float &t, float &u, float &v) const
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];
        v0 = parent->O2WTransform(v0);
        v1 = parent->O2WTransform(v1);
        v2 = parent->O2WTransform(v2);

        Vector3f v0v1 = v1 - v0;
        Vector3f v0v2 = v2 - v0;
        Vector3f pvec = ray.direction.Cross(v0v2);
        float det = v0v1.Dot(pvec);

        if (det > EPS)
        {
            Vector3f tvec = ray.src - v0;
            auto alpha = pvec.Dot(tvec);
            if (alpha < EPS || alpha > det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1);
            auto beta = qvec.Dot(ray.direction);
            if (beta < EPS || alpha + beta > det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2.Dot(qvec) * inv_det;
            if (t < EPS)
            {
                return false;
            }
            u = alpha * inv_det;
            v = beta * inv_det;

            return true;
        }
        else if (det < EPS)
        {
            Vector3f tvec = ray.src - v0;
            auto alpha = pvec.Dot(tvec);
            if (alpha > EPS || alpha < det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1);
            auto beta = qvec.Dot(ray.direction);
            if (beta > EPS || alpha + beta < det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2.Dot(qvec) * inv_det;
            if (t < EPS)
            {
                return false;
            }
            u = alpha * inv_det;
            v = beta * inv_det;

            return true;
        }
        return false;
    }

    const Vector3f Triangle::GetSamplePoint() const
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];
        v0 = parent->O2WTransform(v0);
        v1 = parent->O2WTransform(v1);
        v2 = parent->O2WTransform(v2);

        Vector3f v0v1 = v1 - v0;
        Vector3f v0v2 = v2 - v0;

        float sqr1 = std::sqrt(Random::Float());
        float r2 = Random::Float();

        float a = 1.0f - sqr1;
        float b = (1.0f - r2) * sqr1;

        return v0v1 * a + v0v2 * b + v0;
    }

    const Vector3f Triangle::Tangent() const
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        v0 = parent->O2WTransform(v0);
        v1 = parent->O2WTransform(v1);
        return (v1 - v0).Normalized();
    }

    const Vector3f Triangle::Normal() const
    {
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];
        v1 = parent->O2WTransform(v1);
        v2 = parent->O2WTransform(v2);
        return Tangent().Cross(v2 - v1).Normalized();
    }

    const float Triangle::Area() const
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];
        Vector3f v0v1 = v1 - v0;
        Vector3f v0v2 = v2 - v0;

        return v0v1.Cross(v0v2).Length() * 0.5f;
    }
}