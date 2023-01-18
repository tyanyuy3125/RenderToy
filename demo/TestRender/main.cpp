#include "../../include/OpenPT/openpt.h"

#include <fstream>
#include <iostream>

int main(){
    OpenPT::Vector3f *buffer;
    OpenPT::TestRenderer tr;
    tr.Render(0, buffer);
    OpenPT::PPMExporter ppm_exporter;

    std::ofstream os;
    os.open("./testimage.ppm");
    ppm_exporter.Export(os, buffer);
    os.close();

    std::cout<<"Render completed.\n";
    return 0;
}
