#include <bits/stdc++.h>
#include <RenderToy/rendertoy.h>

using namespace std;
using namespace RenderToy;

int main()
{
    Image img(SizeN(1920, 1080));
    // img.DrawLine({0,0},{200,800});
    auto center = img.resolution.Center();
    for (int deg = 0; deg < 360; ++deg)
    {
        auto current_point = center + PointN(500 * std::cos(Convert::DegreeToRadians(deg)), 500 * std::sin(Convert::DegreeToRadians(deg)));
        img.DrawLine(center, current_point);
    }

    img.Fill({20, 900}, {100, 100}, Vector3f::X);

    std::ofstream os;
    os.open("Drawing.bmp");

    BMPExporter exporter(img);
    exporter.Export(os);

    return 0;
}