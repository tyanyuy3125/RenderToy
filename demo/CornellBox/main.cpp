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
    OBJModelImporter::Import(world, "./cornellbox.obj");
    Vector3f light_color = Convert::BlackBody(6000) * 5.0f;

    PrincipledBSDF mat_red(Vector3f::X);
    PrincipledBSDF mat_green(Vector3f::Y);
    PrincipledBSDF mat_white(Vector3f::White);
    PrincipledBSDF mat_light(Vector3f::White, light_color);
    PrincipledBSDF mat_silver({0.9f, 0.9f, 0.9f}, Vector3f::O, 0.05f, 1.0f, 0.0f, 1.45f, 1.0f);
    PrincipledBSDF mat_glass(2.0f * Vector3f::White, Vector3f::O, 0.0f, 0.0f, 1.0f, 1.33f);

    world.meshes[0]->tex = &mat_white;
    world.meshes[3]->tex = &mat_red;
    world.meshes[4]->tex = &mat_green;
    world.meshes[1]->tex = &mat_white;
    world.meshes[2]->tex = &mat_white;
    world.meshes[5]->tex = &mat_light;
    world.PrepareDirectLightSampling();

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(academy_format);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 1.0f}) * AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-90)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(SizeN(1920, 1080), Vector2f(16.0f, 9.0f)));
    // RenderContext rc(&world, FormatSettings(SizeN(32*8, 18*8), Vector2f(16.0f, 9.0f)));
    // TestRenderer renderer(rc.format_settings.resolution);
    PathTracingRenderer renderer(&rc, 32);
    // DepthBufferRenderer renderer(&rc, 5.0f, 15.0f);
    // NormalRenderer renderer(&rc);
    auto t1 = std::chrono::system_clock::now();
    renderer.Render();
    auto t2 = std::chrono::system_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << std::endl;

    std::cout << "Exporting...\n";

    std::ofstream os;
    os.open("./cornellbox.bmp");
    // os.open("./cornellbox.txt");

    Image img(renderer.render_context);
    // img.GreyScale<Convert::ColorStandard::kITURBT709>();
    // img.EdgeDetection<Orientation::All>();
    // auto newimg = img.Extract([](const Vector3f &_) -> bool
    //                           { return Convert::Luma(_) > 1.0f; });
    // img.Bloom(500, 50.0f, 1.0f);
    // img.GaussianBlur(200, 20.0f);

    BMPExporter exporter(img);
    // ASCIIExporter exporter(img);
    exporter.Export(os);
    // exporter.Export(std::cout);

    os.close();

    std::cout << "Completed.\n";

    return 0;
}
