#include "../../include/OpenPT/openpt.h"

#include <fstream>
#include <iostream>

int main(){
    OpenPT::Vector3f *buffer;
    OpenPT::TestRenderer tr;
    tr.Render(0, buffer);
    OpenPT::BMPExporter exporter;

    std::ofstream os;
    os.open("./testimage.bmp");
    exporter.Export(os, buffer);
    os.close();

    std::cout<<"Render completed.\n";
    return 0;
}
