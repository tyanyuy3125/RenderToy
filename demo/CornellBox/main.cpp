#include "../../include/OpenPT/openpt.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace OpenPT;

int main()
{
    std::cout << "Begin importing...\n";

    World world;
    OBJModelImporter::Import(world, "./cornellbox_rot.obj");

    world.meshes[0]->tex = Material(MaterialType::DIFFUSE,Vector3f::White, Vector3f::O);
    world.meshes[3]->tex = Material(MaterialType::DIFFUSE,Vector3f::X, Vector3f::O);
    world.meshes[5]->tex = Material(MaterialType::DIFFUSE,Vector3f::Y, Vector3f::O);
    world.meshes[1]->tex = Material(MaterialType::DIFFUSE,Vector3f::White, Vector3f::O);
    world.meshes[2]->tex = Material(MaterialType::DIFFUSE,Vector3f::White, Vector3f::O);
    world.meshes[4]->tex = Material(MaterialType::DIFFUSE,Vector3f::White, Convert::BlackBody(6000) * 5.0f);
    // world.sky_emission = Convert::BlackBody(4000) * 0.3f;
    // world.ground_reflection = {0.1,0.09,0.07};
    world.PrepareDirectLightSampling();

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(academy_camera);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 1.0f})*AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-90)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(Size(1280, 720), Vector2f(16.0f, 9.0f)));
    PathTracingRenderer renderer(&rc, 128);
    auto t1 = std::chrono::system_clock::now();
    renderer.Render();
    auto t2 = std::chrono::system_clock::now();
    std::cout<<std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()<<std::endl;

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cornellbox.bmp");

    BMPExporter exporter(&rc);
    exporter.Export(os);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
