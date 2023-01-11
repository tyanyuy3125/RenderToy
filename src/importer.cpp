#include "importer.h"

#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include <tuple>

namespace OpenPT
{
    Importer::Importer(std::string path_) : path(path_) {}

    std::vector<Mesh> Importer::Import()
    {
        std::vector<Mesh> ret;

        Mesh current_mesh;

        std::ifstream fs;
        fs.open(path);

        std::string identifier;
        Vector3f tmp_vec3;
        Vector2f tmp_vec2;
        std::array<std::array<int, 3>, 3> tmp_facedef;
        while (!fs.eof())
        {
            fs >> identifier;
            if (identifier == "o")
            {
                if (!current_mesh.get_name().empty())
                {
                    ret.push_back(current_mesh);
                }
                fs >> current_mesh.get_name();
                continue;
            }

            if (identifier == "v")
            {
                fs >> tmp_vec3;
                current_mesh.get_geo_vert().push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vn")
            {
                fs >> tmp_vec3;
                current_mesh.get_vert_norm().push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vt")
            {
                fs >> tmp_vec2;
                current_mesh.get_vert_tex().push_back(tmp_vec2);
                continue;
            }

            if (identifier == "f")
            {
                for (int i = 0; i < 3; ++i)
                {
                    fs >> tmp_facedef[i][0] >> tmp_facedef[i][1] >> tmp_facedef[i][1];
                }
            }
        }
        ret.push_back(current_mesh);
    }
};