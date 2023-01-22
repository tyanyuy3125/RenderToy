#include "../../include/OpenPT/openpt.h"

#include <fstream>
#include <iostream>

int main(){
    OpenPT::Vector3f *buffer;
    OpenPT::TestRenderer tr(OpenPT::Size(1920, 1080));
    tr.Render();
    OpenPT::BMPExporter exporter(tr.render_context);

    std::ofstream os;
    os.open("./testimage.bmp");
    exporter.Export(os);
    os.close();

    std::cout<<"Render completed.\n";
    return 0;
}
