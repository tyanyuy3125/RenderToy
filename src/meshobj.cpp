#include "meshobj.h"

#define ENABLE_BOUNDING_VOLUME

#include <limits>

namespace OpenPT
{
    // TODO: Optimize the following using BVH and other accelerating methods.
    const bool Mesh::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        auto r_o = ray.src;
        auto r_d = ray.direction;
        t = std::numeric_limits<float>::max();
        bool flag = false;
        for (auto f : faces)
        {
#ifdef ENABLE_BOUNDING_VOLUME
            if (f.bbox.Intersect(ray))
            {
                float t_, u_, v_;
                if (f.Intersect(ray, t_, u_, v_))
                {
                    flag = true;
                    if (t_ < t)
                    {
                        t = t_;
                        u = u_;
                        v = v_;
                    }
                }
            }
#else
            float t_, u_, v_;
            if (f.Intersect(ray, t_, u_, v_))
            {
                flag = true;
                if (t_ < t)
                {
                    t = t_;
                    u = u_;
                    v = v_;
                }
            }
#endif
        }
        return flag;
    }

    const bool IdealSphere::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        return false;
    }

    Triangle::Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_)
        : vert(vert_), norm(norm_), uv(uv_)
    {
        Vector3f bmax = {
            std::max(vert_[0].x, std::max(vert_[1].x, vert_[2].x)),
            std::max(vert_[0].y, std::max(vert_[1].y, vert_[2].y)),
            std::max(vert_[0].z, std::max(vert_[1].z, vert_[2].z))};
        Vector3f bmin = {
            std::min(vert_[0].x, std::min(vert_[1].x, vert_[2].x)),
            std::min(vert_[0].y, std::min(vert_[1].y, vert_[2].y)),
            std::min(vert_[0].z, std::min(vert_[1].z, vert_[2].z))};
        bbox = BoundingBox(bmin, bmax);
    }

    const bool Triangle::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        Vector3f v0 = vert[0];
        Vector3f v1 = vert[1];
        Vector3f v2 = vert[2];

        Vector3f v0v1 = v1 - v0;
        Vector3f v0v2 = v2 - v0;
        Vector3f pvec = ray.direction.Cross(v0v2);
        float det = v0v1.Dot(pvec);

        // Culling enabled.
        if (det < EPS)
        {
            return false;
        }

        Vector3f tvec = ray.src - v0;
        auto alpha = pvec.Dot(tvec);
        if (alpha < 0.0f || alpha > det)
        {
            return false;
        }

        Vector3f qvec = tvec.Cross(v0v1);
        auto beta = qvec.Dot(ray.direction);
        if (beta < 0.0f || alpha + beta > det)
        {
            return false;
        }

        float inv_det = 1.0f / det;
        t = v0v2.Dot(qvec) * inv_det;
        if (t < 0.0f)
        {
            return false;
        }
        u = alpha * inv_det;
        v = beta * inv_det;

        return true;
    }

    BoundingBox::BoundingBox()
        : vmin(Vector3f::O), vmax(Vector3f::O)
    {
    }

    BoundingBox::BoundingBox(const Vector3f &vmin_, const Vector3f &vmax_)
        : vmin(vmin_), vmax(vmax_)
    {
    }

    const bool BoundingBox::Intersect(const Ray &ray)
    {
        float tmin = (vmin.x - ray.src.x) / ray.direction.x;
        float tmax = (vmax.x - ray.src.x) / ray.direction.x;

        if (tmin > tmax)
        {
            std::swap(tmin, tmax);
        }

        float tymin = (vmin.y - ray.src.y) / ray.direction.y;
        float tymax = (vmax.y - ray.src.y) / ray.direction.y;

        if (tymin > tymax)
        {
            std::swap(tymin, tymax);
        }

        if ((tmin > tymax) || (tymin > tmax))
        {
            return false;
        }

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (vmin.z - ray.src.z) / ray.direction.z;
        float tzmax = (vmax.z - ray.src.z) / ray.direction.z;

        if (tzmin > tzmax)
        {
            std::swap(tzmin, tzmax);
        }

        if ((tmin > tzmax) || (tzmin > tmax))
        {
            return false;
        }

        if (tzmin > tmin)
        {
            tmin = tzmin;
        }

        if (tzmax < tmax)
        {
            tmax = tzmax;
        }

        return true;
    }
}