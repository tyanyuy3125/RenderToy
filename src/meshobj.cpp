#include "meshobj.h"

namespace OpenPT
{
    // TODO: return accurate beta & gamma with depth test.
    // TODO: Optimize the following using BVH and other accelerating methods.
    bool Mesh::Intersect(Vector3f r_o, Vector3f r_d)
    {
        // Naive approach.
        for (auto f : face)
        {
            // TODO: The following process might be ill-conditioned.
            Vector3f a = geo_vert[f[0][0] - 1];
            Vector3f b = geo_vert[f[1][0] - 1];
            Vector3f c = geo_vert[f[2][0] - 1];
            // f[0][0]-1 f[1][0]-1 f[2][0]-1
            auto A = Matrix3x3f::Determinant((Matrix3x3f){{a.x - b.x, a.x - c.x, r_d.x},
                                                          {a.y - b.y, a.y - c.y, r_d.y},
                                                          {a.z - b.z, a.z - c.z, r_d.z}});
            auto beta = Matrix3x3f::Determinant((Matrix3x3f){{a.x - r_o.x, a.x - c.x, r_d.x},
                                                             {a.y - r_o.y, a.y - c.y, r_d.y},
                                                             {a.z - r_o.z, a.z - c.z, r_d.z}}) /
                        A;
            auto gamma = Matrix3x3f::Determinant((Matrix3x3f){{a.x - b.x, a.x - r_o.x, r_d.x},
                                                              {a.y - b.y, a.y - r_o.y, r_d.y},
                                                              {a.z - b.z, a.z - r_o.z, r_d.z}}) /
                         A;
            // auto t = Matrix3x3f::Determinant((Matrix3x3f){{a.x - b.x, a.x - c.x, a.x - r_o.x},
            //                                               {a.y - b.y, a.y - c.y, a.y - r_o.y},
            //                                               {a.z - b.z, a.z - c.z, a.z - r_o.z}}) /
            //          A;
            if (beta + gamma <= 1.0f && beta >= 0.0f && gamma >= 0.0f)
            {
                return true;
                break;
            }
        }
    }
}