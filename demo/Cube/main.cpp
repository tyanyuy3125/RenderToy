#include "../../src/importer.h"
#include "../../src/meshobj.h"

#include <vector>
#include <iostream>

int main(){
    std::cout<<"Begin importing...\n";
    OpenPT::Importer importer("./cube.obj");
    std::vector<OpenPT::Mesh> meshes = importer.Import();
    std::cout<<"Imported "<<meshes.size()<<" mesh(es).\n";
    return 0;
}
