#include "meshobj.h"

#include <limits>
#include <cmath>
#include <iostream>

namespace RenderToy
{
    const BoundingBox Triangle::BBox() const
    {
        Vector3f v0 = vert_w[0];
        Vector3f v1 = vert_w[1];
        Vector3f v2 = vert_w[2];

        Vector3f bmax = {
            std::max(v0.x, std::max(v1.x, v2.x)),
            std::max(v0.y, std::max(v1.y, v2.y)),
            std::max(v0.z, std::max(v1.z, v2.z))};
        Vector3f bmin = {
            std::min(v0.x, std::min(v1.x, v2.x)),
            std::min(v0.y, std::min(v1.y, v2.y)),
            std::min(v0.z, std::min(v1.z, v2.z))};

        return BoundingBox(bmin, bmax);
    }

    Triangle::Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_)
        : vert(vert_), vert_w(vert_), norm(norm_), uv(uv_), parent(parent_)
    {
        UpdateCache();
    }

    const bool Triangle::Intersect(const Ray &ray, float &t, float &u, float &v) const
    {
#ifdef ENABLE_CULLING
        Vector3f pvec = ray.direction.Cross(v0v2_w);
        float det = v0v1_w.Dot(pvec);

        if (det < EPS)
        {
            return false;
        }

        Vector3f tvec = ray.src - vert_w[0];
        auto alpha = pvec.Dot(tvec);
        if (alpha < EPS || alpha > det)
        {
            return false;
        }

        Vector3f qvec = tvec.Cross(v0v1_w);
        auto beta = qvec.Dot(ray.direction);
        if (beta < EPS || alpha + beta > det)
        {
            return false;
        }

        float inv_det = 1.0f / det;
        t = v0v2_w.Dot(qvec) * inv_det;
        if (t < EPS)
        {
            return false;
        }
        u = alpha * inv_det;
        v = beta * inv_det;

        return true;
#else
        Vector3f pvec = ray.direction.Cross(v0v2_w);
        float det = v0v1_w.Dot(pvec);

        if (det > EPS)
        {
            Vector3f tvec = ray.src - vert_w[0];
            auto alpha = pvec.Dot(tvec);
            if (alpha < EPS || alpha > det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1_w);
            auto beta = qvec.Dot(ray.direction);
            if (beta < EPS || alpha + beta > det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2_w.Dot(qvec) * inv_det;
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
            Vector3f tvec = ray.src - vert_w[0];
            auto alpha = pvec.Dot(tvec);
            if (alpha > EPS || alpha < det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1_w);
            auto beta = qvec.Dot(ray.direction);
            if (beta > EPS || alpha + beta < det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2_w.Dot(qvec) * inv_det;
            if (t < EPS)
            {
                return false;
            }
            u = alpha * inv_det;
            v = beta * inv_det;

            return true;
        }
        return false;
#endif
    }

    const Vector3f Triangle::GetSamplePoint() const
    {
        float sqr1 = std::sqrt(Random::Float());
        float r2 = Random::Float();

        float a = 1.0f - sqr1;
        float b = (1.0f - r2) * sqr1;

        return v0v1_w * a + v0v2_w * b + vert_w[0];
    }

    const Vector3f Triangle::Tangent() const
    {
        return v0v1_w.Normalized();
    }

    const Vector3f Triangle::Normal() const
    {
        Vector3f v1 = vert_w[1];
        Vector3f v2 = vert_w[2];
        return Tangent().Cross(v2 - v1).Normalized();
    }

    const float Triangle::Area() const
    {
        return v0v1_w.Cross(v0v2_w).Length() * 0.5f;
    }

    const float Triangle::AreaC() const
    {
        return area;
    }

    const Vector3f Triangle::TangentC() const
    {
        return tangent;
    }

    const Vector3f Triangle::NormalC() const
    {
        return normal;
    }

    void Triangle::UpdateCache()
    {
        v0v1_w = vert_w[1] - vert_w[0];
        v0v2_w = vert_w[2] - vert_w[0];
        area = Area();
        normal = Normal();
        tangent = Tangent();
    }

    void Mesh::SetO2W(const Matrix4x4f &object_to_world_)
    {
        GeoObj::SetO2W(object_to_world_);
        for (auto tri : faces)
        {
            tri->vert_w[0] = O2WTransform(tri->vert[0]);
            tri->vert_w[1] = O2WTransform(tri->vert[1]);
            tri->vert_w[2] = O2WTransform(tri->vert[2]);
            tri->UpdateCache();
        }
    }
}