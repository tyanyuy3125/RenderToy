#include "importer.h"
#include "meshobj.h"
#include "mathfunc.h"

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <tuple>
#include <limits>

namespace OpenPT
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
        while (!fs.eof())
        {
            fs >> identifier;
            if (identifier == "o")
            {
                if (current_mesh!=nullptr)
                {
                    world.meshes.push_back(current_mesh);
                    // vert.clear();
                    // norm.clear();
                    // uv.clear();
                    // vert.push_back(Vector3f::O);
                    // norm.push_back(Vector3f::O);
                    // uv.push_back(Vector2f::O);
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

            if (identifier == "f")
            {
                // f vert/uv/norm
                fs >> a >> slash >> b >> slash >> c >>
                    d >> slash >> e >> slash >> f >>
                    g >> slash >> h >> slash >> i;

                world.triangles.push_back(new Triangle({vert[a], vert[d], vert[g]}, {norm[c], norm[f], norm[i]}, {uv[b], uv[e], uv[h]}, current_mesh));
                current_mesh->faces.push_back(world.triangles.back());
                continue;
            }

            fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        world.meshes.push_back(current_mesh);
    }
};