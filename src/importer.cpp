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
        char slash;
        std::array<std::array<int, 3>, 3> tmp_facedef;
        while (!fs.eof())
        {
            fs >> identifier;
            if (identifier == "o")
            {
                if (!current_mesh.name.empty())
                {
                    ret.push_back(current_mesh);
                }
                fs >> current_mesh.name;
                continue;
            }

            if (identifier == "v")
            {
                fs >> tmp_vec3;
                current_mesh.geo_vert.push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vn")
            {
                fs >> tmp_vec3;
                current_mesh.vert_norm.push_back(tmp_vec3);
                continue;
            }

            if (identifier == "vt")
            {
                fs >> tmp_vec2;
                current_mesh.vert_tex.push_back(tmp_vec2);
                continue;
            }

            if (identifier == "f")
            {
                for (int i = 0; i < 3; ++i)
                {
                    fs >> tmp_facedef[i][0] >> slash >> tmp_facedef[i][1] >> slash >> tmp_facedef[i][2];
                }
                current_mesh.face.push_back(tmp_facedef);
                continue;
            }

            fs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        ret.push_back(current_mesh);

        return ret;
    }
};