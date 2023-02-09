#include "../../include/RenderToy/rendertoy.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace RenderToy;

int main()
{
    std::cout << "Begin importing...\n";

    World world;
    OBJModelImporter::Import(world, "./ss.obj");
    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(academy_format);
    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-60)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);
    std::cout << "Begin rendering...\n";
    RenderContext rc(&world, FormatSettings(SizeN(1920, 1080), Vector2f(16.0f, 9.0f)));
    NormalRenderer renderer(&rc);
    // DepthBufferRenderer renderer(&rc, 5.0f, 20.0f);
    auto t1 = std::chrono::system_clock::now();
    renderer.Render();
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;
    std::cout << "Exporting...\n";
    std::ofstream os;
    os.open("./ss.bmp");
    Image img(renderer.render_context);
    BMPExporter exporter(img);
    exporter.Export(os);
    os.close();
    std::cout << "Completed.\n";

    return 0;
}
