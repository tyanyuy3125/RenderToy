#include "meshobj.h"

#include <limits>

#define CULLING

namespace OpenPT
{
    // TODO: Implement M-T Algorithm.
    // TODO: Optimize the following using BVH and other accelerating methods.
    const bool Mesh::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        auto r_o = ray.src;
        auto r_d = ray.direction;
        // t = std::numeric_limits<float>::max();
        bool flag = false;
        for (auto f : face)
        {
            Vector3f a = geo_vert[f[0][0] - 1];
            Vector3f b = geo_vert[f[1][0] - 1];
            Vector3f c = geo_vert[f[2][0] - 1];

            Vector3f v0v1 = b - a;
            Vector3f v0v2 = c - a;
            Vector3f N = v0v1.Cross(v0v2);

            #ifdef CULLING
            if (Vector3f::Dot(r_d, N) > 0)
            {
                continue;
            }
            #endif

            float denom = N.Dot(N);
            float NdotRayDirection = N.Dot(r_d);

            if (std::abs(NdotRayDirection) < EPS)
            {
                continue;
            }
            float d = -N.Dot(a);
            t = -(N.Dot(r_o) + d) / NdotRayDirection;
            if (t < 0)
            {
                continue;
            }
            Vector3f P = r_o + t * r_d;
            Vector3f C;
            Vector3f edge0 = b - a;
            Vector3f vp0 = P - a;
            C = edge0.Cross(vp0);
            if (N.Dot(C) < 0)
            {
                continue;
            }

            Vector3f edge1 = c - b;
            Vector3f vp1 = P - b;
            C = edge1.Cross(vp1);
            if ((u = N.Dot(C)) < 0)
            {
                continue;
            }

            Vector3f edge2 = a - c;
            Vector3f vp2 = P - c;
            C = edge2.Cross(vp2);
            if ((v = N.Dot(C)) < 0)
            {
                continue;
            }

            u /= denom;
            v /= denom;

            flag = true;
            continue;
        }
        return flag;
    }

    const bool IdealSphere::Intersect(const Ray &ray, float &t, float &u, float &v)
    {
        return false;
    }
}