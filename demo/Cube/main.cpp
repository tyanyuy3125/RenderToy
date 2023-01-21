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

    // Matrix4x4f camera_mat4 = {{1.0f, 0.0f, 0.0f, 0.0f},
    //                           {0.0f, 0.0f, -1.0f, -10.0f},
    //                           {0.0f, 1.0f, 0.0f, 0.0f},
    //                           {0.0f, 0.0f, 0.0f, 1.0f}};
    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(45), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout
        << "Begin rendering...\n";

    Vector3f *buffer;
    IntersectTestRenderer renderer(&world);
    renderer.format_settings.resolution = Size(1280, 720);
    clock_t timeStart = clock();
    renderer.Render(0, buffer);
    clock_t timeEnd = clock();
    printf("Render time: %04.2f (sec)\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cube.bmp");

    BMPExporter exporter;
    exporter.format_settings.resolution = Size(1280, 720);
    exporter.Export(os, buffer);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
