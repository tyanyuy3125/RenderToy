#define BUFFER(x, y, width) buffer[y * width + x]

#include <algorithm>

#include "mathfunc.h"
#include "exporter.h"

namespace OpenPT
{
    void PPMExporter::Export(std::ostream &os, Vector3f *buffer)
    {
        int nx = format_settings.resolution.width;
        int ny = format_settings.resolution.height;
        os << "P3" << std::endl
                   << nx << ' ' << ny << std::endl
                   << "255" << std::endl;
        for (int i = 0; i < ny; ++i)
        {
            for (int j = 0; j < nx; ++j)
            {
                auto real_color = BUFFER(j, i, nx);
                float r = std::clamp(real_color.x, 0.0f, 1.0f);
                float g = std::clamp(real_color.y, 0.0f, 1.0f);
                float b = std::clamp(real_color.z, 0.0f, 1.0f);
                int ir = int(255.99f * r);
                int ig = int(255.99f * g);
                int ib = int(255.99f * b);
                os << ir << ' ' << ig << ' ' << ib << '\n';
            }
        }
    }
}