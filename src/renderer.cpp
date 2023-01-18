#define BUFFER(x, y, width) buffer[y * width + x]

#include "renderer.h"

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
