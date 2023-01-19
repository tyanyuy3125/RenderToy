#include "../../include/OpenPT/openpt.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace OpenPT;

int main()
{
    std::cout << "Begin importing...\n";
    OBJModelImporter importer("./cube.obj");
    std::vector<Mesh> meshes = importer.Import();
    std::cout << "Imported " << meshes.size() << " mesh(es).\n";

    World world;
    world.meshes.insert(world.meshes.end(), meshes.begin(), meshes.end());
    world.cameras.push_back(AcademyCamera);

    Matrix4x4f camera_mat4 = {{1.0f, 0.0f, 0.0f, 0.0f},
                              {0.0f, 0.0f, -1.0f, -10.0f},
                              {0.0f, 1.0f, 0.0f, 0.0f},
                              {0.0f, 0.0f, 0.0f, 1.0f}};
    world.cameras[0].SetO2W(camera_mat4);

    std::cout
        << "Begin rendering...\n";

    Vector3f *buffer;
    IntersectTestRenderer renderer(&world);
    renderer.Render(0, buffer);

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cube.bmp");

    BMPExporter exporter;
    exporter.Export(os, buffer);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
