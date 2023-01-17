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

    void IntersectTestRenderer::Render(int camera_id, Vector2f resolution)
    {
        // Not implemented
    }
}
