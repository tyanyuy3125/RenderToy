#include "meshobj.h"

#include <limits>

namespace OpenPT
{
    // TODO: Optimize the following using BVH and other accelerating methods.
    const bool Mesh::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        auto r_o = ray.src;
        auto r_d = ray.direction;
        // t = std::numeric_limits<float>::max();
        bool flag = false;
        for (auto f : face)
        {
            Vector3f v0 = geo_vert[f[0][0] - 1];
            Vector3f v1 = geo_vert[f[1][0] - 1];
            Vector3f v2 = geo_vert[f[2][0] - 1];

            Vector3f v0v1 = v1-v0;
            Vector3f v0v2 = v2-v0;
            Vector3f pvec = r_d.Cross(v0v2);
            float det = v0v1.Dot(pvec);

            if(det < EPS) continue;

            Vector3f tvec = r_o - v0;
            auto alpha = pvec.Dot(tvec);
            if(alpha<0.0f || alpha > det) continue;

            Vector3f qvec = tvec.Cross(v0v1);
            auto beta = qvec.Dot(r_d);
            if(beta < 0.0f || alpha+beta > det) continue;

            float inv_det = 1.0f / det;
            t = v0v2.Dot(qvec) * inv_det;
            if(t < 0.0f){
                continue;
            }
            flag = true;
        }
        return flag;
    }

    const bool IdealSphere::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        return false;
    }
}