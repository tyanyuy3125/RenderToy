#include <bits/stdc++.h>
#include <OpenImageDenoise/oidn.hpp>
#include <RenderToy/rendertoy.h>

using namespace RenderToy;

auto main() -> int
{
    oidn::DeviceRef device = oidn::newDevice();
    device.commit();

    World world;
    OBJModelImporter::Import(world, "./monkey.obj");

    PrincipledBSDF mat_light(Vector3f::White, Vector3f::White * 5.0f);
    PrincipledBSDF mat_ground(Vector3f::White);
    PrincipledBSDF mat_monkey(Vector3f::White, Vector3f::O, 0.1f, 1.0f);

    world.meshes[0]->tex = &mat_ground;
    world.meshes[1]->tex = &mat_monkey;
    world.meshes[2]->tex = &mat_light;
    world.meshes[3]->tex = &mat_light;

    world.PrepareDirectLightSampling();

    std::cout << "Imported " << world.triangles.size() << " triangle(s) and " << world.meshes.size() << " mesh(es).\n";
    world.cameras.push_back(academy_format);

    Matrix4x4f camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 10.0f});
    camera_mat4 = AffineTransformation::Translation({0.0f, 0.0f, 1.0f}) * AffineTransformation::RotationEulerXYZ({0.0f, 0.0f, Convert::DegreeToRadians(-90)}) * AffineTransformation::RotationEulerXYZ({Convert::DegreeToRadians(90), 0.0f, 0.0f}) * camera_mat4;
    world.cameras[0].SetO2W(camera_mat4);

    std::cout << "Begin rendering...\n";

    RenderContext rc(&world, FormatSettings(SizeN(1280, 720), Vector2f(16.0f, 9.0f)));
    PathTracingRenderer renderer(&rc, 16);
    renderer.Render();
    // DepthBufferRenderer renderer(&rc, 5.0f, 15.0f);
    // NormalRenderer uv_renderer(&rc);
    // uv_renderer.Render();
    // AlbedoRenderer albedo_renderer(&rc);
    std::cout << "Exporting original...\n";

    std::ofstream os;
    os.open("./monkey_original.bmp");

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
    filter.setImage("color", img.buffer, oidn::Format::Float3, 1280, 720); // beauty
    // filter.setImage("albedo", albedoPtr, oidn::Format::Float3, width, height); // auxiliary
    // filter.setImage("normal", normalPtr, oidn::Format::Float3, width, height); // auxiliary
    filter.setImage("output", oidn_output.buffer, oidn::Format::Float3, 1280, 720); // denoised beauty
    filter.set("hdr", true);                                                        // beauty image is HDR
    filter.commit();

    // Filter the image
    filter.execute();

    // Check for errors
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;

    std::cout << "Exporting denoised...\n";

    os.open("./monkey_denoised.bmp");
    BMPExporter denoised_exporter(oidn_output);
    denoised_exporter.Export(os);
    os.close();
    std::cout << "Completed.\n";

    return 0;
}