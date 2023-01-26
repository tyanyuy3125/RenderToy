#include "../../include/RenderToy/rendertoy.h"

#include <fstream>
#include <iostream>

int main(){
    RenderToy::Vector3f *buffer;
    RenderToy::TestRenderer tr(RenderToy::Size(1920, 1080));
    tr.Render();
    RenderToy::BMPExporter exporter(tr.render_context);

    std::ofstream os;
    os.open("./testimage.bmp");
    exporter.Export(os);
    os.close();

    std::cout<<"Render completed.\n";
    return 0;
}
