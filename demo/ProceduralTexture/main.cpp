#include "../../include/RenderToy/rendertoy.h"

#include <bits/stdc++.h>

using namespace RenderToy;

auto main() -> int
{
    ProceduralTexture::CheckerBoard<float, float> cb(8);
    auto img = cb.Rasterize(800);
    
    std::ofstream os;
    os.open("./checkboard.bmp");
    BMPExporter exporter(img);
    exporter.Export(os);
    os.close();

    ProceduralTexture::Wave<float, float> wave(8);
    img = wave.Rasterize(800);
    os.open("./wave.bmp");
    BMPExporter exporter2(img);
    exporter2.Export(os);
    os.close();

    return 0;
}