#include "meshobj.h"

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
            float t_, u_, v_;
            if(f.Intersect(ray, t_, u_, v_)){
                flag = true;
                if(t_ < t){
                    t = t_;
                    u = u_;
                    v = v_;
                }
            }
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
}