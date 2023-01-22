#include "meshobj.h"

#define ENABLE_BOUNDING_VOLUME
#define ENABLE_MESH_BVH

#include <limits>

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