#include "../../include/OpenPT/openpt.h"

#include <vector>
#include <iostream>
#include <fstream>

using namespace OpenPT;

int main()
{
    std::cout << "Begin importing...\n";

    World world;
    OBJModelImporter::Import(world, "./cube.obj");

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and "<<world.meshes.size()<<" mesh(es).\n";
    world.cameras.push_back(AcademyCamera);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(45)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(45), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout
        << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(Size(1280, 720), Vector2f(16.0f, 9.0f)));
    IntersectTestRenderer renderer(&rc);
    clock_t timeStart = clock();
    renderer.Render();
    clock_t timeEnd = clock();
    printf("Render time: %04.2f (sec)\n", (float)(timeEnd - timeStart) / CLOCKS_PER_SEC);

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cube.bmp");

    BMPExporter exporter(&rc);
    exporter.Export(os);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
