#include "meshobj.h"

namespace OpenPT
{
    std::string &Mesh::get_name()
    {
        return name;
    }
    std::vector<Vector3f> &Mesh::get_geo_vert()
    {
        return geo_vert;
    }
    std::vector<Vector3f> &Mesh::get_vert_norm()
    {
        return vert_norm;
    }
    std::vector<Vector2f> &Mesh::get_vert_tex()
    {
        return vert_tex;
    }
};