#define BUFFER(x, y, width) render_context->buffer[y * width + x]

#include "renderer.h"
#include "camera.h"
#include "ray.h"
#include "bvh.h"

namespace OpenPT
{
    IntersectTestRenderer::IntersectTestRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void IntersectTestRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);

        // BEGIN Camera Preparation
        float top = (Convert::InchToMM(cam->gate_dimension.y) / 2.0f) / cam->focal_length;
        float right = (Convert::InchToMM(cam->gate_dimension.x) / 2.0f) / cam->focal_length;
        // We implement FILL CONVENTION when aspect ratio conflicts with gate dimension.
        // Corresponding to AUTO setting of Sensor Fit in Blender.
        float gate_aspr = cam->gate_dimension.x / cam->gate_dimension.y;
        float film_aspr = render_context->format_settings.resolution.AspectRatio();
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
        // END Camera Preparation


        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) / float(render_context->format_settings.resolution.width), float(y) / float(render_context->format_settings.resolution.height)};
                Vector2f screen_coord = {2 * right * NDC_coord.x - right, 2 * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));

                // TODO: simplify
                cast_ray.direction = cam->O2WTransform(cast_ray.direction);
                cast_ray.src = cam->O2WTransform(cast_ray.src);
                cast_ray.direction -= cast_ray.src; // Temporal solution!
                cast_ray.direction.Normalize();     // Temporal solution!

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = (1 - u - v) * intersected->norm[0] + u * intersected->norm[1] + v * intersected->norm[2];
                    // BUFFER(x, y, format_settings.resolution.width) = (1 - u - v) * Vector3f::X + u * Vector3f::Y + v * Vector3f::Z;
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }

    TestRenderer::TestRenderer()
        : IRenderer(nullptr)
    {
    }

    void TestRenderer::Render()
    {
        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(float(x) / render_context->format_settings.resolution.width, 1.0f, 1.0f);
            }
        }
    }

    FormatSettings::FormatSettings(Size resolution_, Vector2f aspect_)
        : resolution(resolution_), aspect(aspect_)
    {
    }
    
    RenderContext::RenderContext(World *world_, FormatSettings format_settings_)
        : world(world_), format_settings(format_settings_)
    {
        bvh = new BVH(world->triangles);
        buffer = new Vector3f[format_settings.resolution.Area()];
    }

    RenderContext::~RenderContext()
    {
        delete[] buffer;
        delete bvh;
    }

    IRenderer::IRenderer(RenderContext *render_context_)
        : render_context(render_context_)
    {
    }
    
}
