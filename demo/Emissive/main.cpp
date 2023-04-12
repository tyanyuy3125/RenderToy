#include <bits/stdc++.h>
#include <OpenImageDenoise/oidn.hpp>
#include <RenderToy/rendertoy.h>

using namespace RenderToy;

auto main() -> int
{
    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    World world;
    // OBJModelImporter::Import(world, "./emissive.obj");
    OBJModelImporter::Import(world, "./room.obj");

    PrincipledBSDF mat_red(Vector3f::X);
    PrincipledBSDF mat_green(Vector3f::Y);
    PrincipledBSDF mat_white(Vector3f(224.0f/255.0f, 218.0f/255.0f, 167.0f/255.0f));
    PrincipledBSDF mat_light(Vector3f::White, Vector3f::White * 5.0f);
    PrincipledBSDF mat_light2(Vector3f::X, Convert::BlackBody(4500) * 5.0f);
    PrincipledBSDF mat_light3(Vector3f::X, Convert::BlackBody(9000) * 5.0f);
    PrincipledBSDF mat_light10(Vector3f::X, Convert::BlackBody(20000) * 5.0f);
    PrincipledBSDF mat_silver({0.9f, 0.9f, 0.9f}, Vector3f::O, 0.05f, 1.0f, 0.0f, 1.45f, 1.0f);
    PrincipledBSDF mat_glass(1.0f * Vector3f::White, Vector3f::O, 0.0f, 0.0f, 1.0f, 1.33f);

    // world.meshes[0]->tex = &mat_white;
    // world.meshes[1]->tex = &mat_light;
    // world.meshes[2]->tex = &mat_white;
    // world.meshes[3]->tex = &mat_light2;
    // world.meshes[4]->tex = &mat_light3;
    // world.meshes[5]->tex = &mat_silver;
    // world.meshes[6]->tex = &mat_light10;
    // world.meshes[7]->tex = &mat_white;
    world.meshes[0]->tex = &mat_white;
    world.meshes[1]->tex = &mat_white;
    world.meshes[2]->tex = &mat_white;
    world.meshes[3]->tex = &mat_white;
    world.meshes[4]->tex = &mat_white;
    world.meshes[5]->tex = &mat_light;

    world.PrepareDirectLightSampling();

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(academy_format);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 20.0f});
    camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 1.0f}) * AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-130)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(SizeN(1280, 720), Vector2f(16.0f, 9.0f)));
    PathTracingRenderer renderer(&rc, 64);
    renderer.Render();
    // DepthBufferRenderer renderer(&rc, 5.0f, 15.0f);
    // NormalRenderer uv_renderer(&rc);
    // uv_renderer.Render();
    // AlbedoRenderer albedo_renderer(&rc);
    std::cout << "Exporting original...\n";

    std::ofstream os;
    os.open("./original.bmp");

    Image img(renderer.render_context);
    img.Tonemap();
    img.GammaCorrection();
    BMPExporter exporter(img);
    exporter.Export(os);
    os.close();
    std::cout << "Completed.\n";

    std::cout << "Denoising...\n";

    Image oidn_output(SizeN(1280, 720));

    oidn::FilterRef filter = device.newFilter("RT");
    filter.setImage("color", img.GetBuffer(), oidn::Format::Float3, 1280, 720); // beauty
    // filter.setImage("albedo", albedoPtr, oidn::Format::Float3, width, height); // auxiliary
    // filter.setImage("normal", normalPtr, oidn::Format::Float3, width, height); // auxiliary
    filter.setImage("output", oidn_output.GetBuffer(), oidn::Format::Float3, 1280, 720); // denoised beauty
    filter.set("hdr", true);                                                             // beauty image is HDR
    filter.commit();

    // Filter the image
    filter.execute();

    // Check for errors
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;

    std::cout << "Exporting denoised...\n";

    os.open("./denoised.bmp");
    BMPExporter denoised_exporter(oidn_output);
    denoised_exporter.Export(os);
    os.close();
    std::cout << "Completed.\n";

    return 0;
}