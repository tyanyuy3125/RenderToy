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
    img.Fill({100, 900}, {200, 100}, Vector3f::Y);
    img.Fill({200, 900}, {300, 100}, Vector3f::Z);
    img.Binarize<Convert::ColorStandard::kITURBT601>(0.5f);
    // img.GaussianBlur(200, 20.0f);
    // img.GreyScale<Convert::ColorStandard::kITURBT2020>();

    // for(int i=0;i<1920;++i)
    // {
    //     for(int j=0;j<1080;++j)
    //     {
    //         img(i,j)=Convert::BlackBody((float(i)/1920.0f) * 15000.0f);
    //     }
    // }

    std::ofstream os;
    os.open("Drawing.bmp");

    BMPExporter exporter(img);
    exporter.Export(os);
    os.close();

    return 0;
}