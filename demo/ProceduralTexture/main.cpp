#include <RenderToy/rendertoy.h>

#include <bits/stdc++.h>

using namespace RenderToy;

auto main() -> int
{
    ProceduralTexture::CheckerBoard cb(8);
    auto img = cb.Rasterize(800);
    
    std::ofstream os;
    os.open("./checkboard.bmp");
    BMPExporter exporter(img);
    exporter.Export(os);
    os.close();

    ProceduralTexture::Wave wave(8);
    img = wave.Rasterize(800);
    os.open("./wave.bmp");
    BMPExporter exporter2(img);
    exporter2.Export(os);
    os.close();

    ProceduralTexture::PerlinNoise pn(114514);
    img = pn.Rasterize(800);
    img.Binarize<Convert::ColorStandard::kITURBT709>(0.5f);
    os.open("./pn.bmp");
    BMPExporter exporter3(img);
    exporter3.Export(os);
    os.close();

    return 0;
}