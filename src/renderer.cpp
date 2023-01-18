#define BUFFER(x, y, width) buffer[y * width + x]

#include "renderer.h"
#include "camera.h"
#include "ray.h"

namespace OpenPT
{
    IRenderer::IRenderer(World *world_)
        : world(world_)
    {
    }

    IntersectTestRenderer::IntersectTestRenderer(World *world_)
        : IRenderer(world_)
    {
    }

    void IntersectTestRenderer::Render(int camera_id, Vector3f *&buffer)
    {
        Camera *cam = &(world->cameras[camera_id]);
        buffer = new Vector3f[format_settings.resolution.Area()];


        // CAMERA PREPARATION

        float top = (InchToMM(cam->gate_dimension.y) / 2.0f) / cam->focal_length;
        float right = (InchToMM(cam->gate_dimension.x) / 2.0f) / cam->focal_length;

        // TODO: Verify if following comments are telling truth.
        // We implement FILL CONVENTION when aspect ratio conflicts with gate dimension.
        // Corresponding to AUTO setting of Sensor Fit in Blender.
        float gate_aspr = cam->gate_dimension.x / cam->gate_dimension.y;
        float film_aspr = format_settings.resolution.AspectRatio();

        float xscale = 1.0f, yscale = 1.0f;

        if (gate_aspr >= film_aspr)
        {
            // Gate is wider than Film.
            xscale = film_aspr / gate_aspr;
        }
        else
        {
            // Film is wider than Gate.
            yscale = gate_aspr / film_aspr;
        }

        right *= xscale;
        top *= yscale;

        for (int y = 0; y < format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) / float(format_settings.resolution.width), float(y) / float(format_settings.resolution.height)};
                Vector2f screen_coord = {2 * right * NDC_coord.x - right, 2 * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));

                BUFFER(x, y, format_settings.resolution.width) = Vector3f(float(x) / format_settings.resolution.width, 1.0f, 1.0f);
            }
        }
    }

    TestRenderer::TestRenderer()
        : IRenderer(nullptr)
    {
    }

    void TestRenderer::Render(int camera_id, Vector3f *&buffer)
    {
        // TODO: Memory leak!
        buffer = new Vector3f[format_settings.resolution.Area()];
        for (int y = 0; y < format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < format_settings.resolution.width; ++x)
            {
                BUFFER(x, y, format_settings.resolution.width) = Vector3f(float(x) / format_settings.resolution.width, 1.0f, 1.0f);
            }
        }
    }

    FormatSettings::FormatSettings(Size resolution_, Vector2f aspect_)
        : resolution(resolution_), aspect(aspect_)
    {
    }
}
