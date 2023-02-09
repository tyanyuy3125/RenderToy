#include "importer.h"
#include "object.h"
#include "rtmath.h"

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <tuple>
#include <limits>
#include <vector>
#include <regex>

static const std::vector<std::string> Split(const std::string &str, const char delimiter)
{
    std::vector<std::string> ret;
    std::istringstream str_ss(str);
    std::string tmp;
    while (std::getline(str_ss, tmp, delimiter))
    {
        ret.push_back(tmp);
    }
    return ret;
}

namespace RenderToy
{
    void OBJModelImporter::Import(World &world, const std::string &path)
    {
        Mesh *current_mesh = nullptr;

        std::ifstream fs;
        fs.open(path);

        std::string identifier;
        Vector3f tmp_vec3;
        Vector2f tmp_vec2;
        char slash;

        std::vector<Vector3f> vert;
        std::vector<Vector3f> norm;
        std::vector<Vector2f> uv;
        vert.push_back(Vector3f::O);
        norm.push_back(Vector3f::O);
        uv.push_back(Vector2f::O);

        int a, b, c, d, e, f, g, h, i;
        while (fs >> identifier)
        {
            if (identifier == "o")
            {
                if (current_mesh != nullptr)
                {
                    world.meshes.push_back(current_mesh);
                }
                current_mesh = new Mesh();
                fs >> current_mesh->name;
                continue;
            }

            if (identifier == "v")
            {
                fs >> tmp_vec3;
                vert.push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vn")
            {
                fs >> tmp_vec3;
                norm.push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vt")
            {
                fs >> tmp_vec2;
                uv.push_back(tmp_vec2);
                continue;
            }

            if (identifier == "s")
            {
                fs >> current_mesh->smooth;
                continue;
            }

            if (identifier == "f")
            {
                // f vert/uv/norm
                // fs >> a >> slash >> b >> slash >> c >>
                //     d >> slash >> e >> slash >> f >>
                //     g >> slash >> h >> slash >> i;

                // world.triangles.push_back(new Triangle({vert[a], vert[d], vert[g]}, {norm[c], norm[f], norm[i]}, {uv[b], uv[e], uv[h]}, current_mesh));
                // current_mesh->tris.push_back(world.triangles.back());
                std::string face_def_str;
                std::getline(fs, face_def_str);
                face_def_str = std::regex_replace(face_def_str, std::regex("^ +| +$|( ) +"), "$1");
                auto vert_defs = Split(face_def_str, ' ');
                std::vector<Vector3f> poly_vert;
                std::vector<Vector3f> poly_norm;
                std::vector<Vector2f> poly_uv;

                std::for_each(vert_defs.begin(), vert_defs.end(),
                              [&vert, &norm, &uv, &poly_vert, &poly_norm, &poly_uv](std::string &_) -> void
                              {
                                  auto unslashed = Split(_, '/');
                                  std::array<std::optional<int>, 3> unslashed_num;
                                  std::transform(unslashed.begin(), unslashed.end(), unslashed_num.begin(), [](std::string &__) -> std::optional<int>
                                                 {
                                                    if(__.empty())
                                                    {
                                                        return {};
                                                    }
                                                    try
                                                    {
                                                    int ret_num = std::stoi(__);
                                                    return ret_num;
                                                    }
                                                    catch(...)
                                                    {
                                                        return {};
                                                    } });
                                  poly_vert.push_back(unslashed_num[0].has_value() ? vert[unslashed_num[0].value()] : Vector3f::O);
                                  poly_norm.push_back(unslashed_num[2].has_value() ? norm[unslashed_num[2].value()] : Vector3f::O);
                                  poly_uv.push_back(unslashed_num[1].has_value() ? uv[unslashed_num[1].value()] : Vector2f::O);
                              });

                Polygon poly(poly_vert.size(), poly_vert, poly_norm, poly_uv, current_mesh);
                auto tris = poly.ConvertToTriangle();
                world.triangles.insert(world.triangles.end(), tris.begin(), tris.end());
                current_mesh->tris.insert(current_mesh->tris.begin(), tris.begin(), tris.end());
                continue;
            }

            fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        world.meshes.push_back(current_mesh);
    }
};