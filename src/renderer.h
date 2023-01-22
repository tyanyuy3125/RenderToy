#ifndef RENDERER_H
#define RENDERER_H

#include <ostream>

#include "world.h"
#include "mathfunc.h"
#include "bvh.h"

namespace OpenPT
{
    // Use Blender conventions.
    struct FormatSettings
    {
        Size resolution = Size(1920, 1080);
        Vector2f aspect = Vector2f(1.0f, 1.0f); // Don't touch it!

        FormatSettings() = default;
        FormatSettings(Size resolution_, Vector2f aspect_);
    };

    struct RenderContext
    {
        World *world;
        BVH *bvh;
        Vector3f *buffer;

        FormatSettings format_settings;
        int camera_id = 0;

        RenderContext(World *world_, FormatSettings format_settings_);
        ~RenderContext();
    };

    class IRenderer
    {
    protected:
        RenderContext *render_context;

    public:
        IRenderer() = delete;
        IRenderer(const IRenderer &) = delete;
        IRenderer(const IRenderer &&) = delete;
        IRenderer(RenderContext *render_context_);
        virtual void Render() = 0;
    };

    class TestRenderer : public IRenderer
    {
    public:
        TestRenderer();
        virtual void Render() override final;
    };

    class IntersectTestRenderer : public IRenderer
    {
    public:
        IntersectTestRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };

    class DepthBufferRenderer : public IRenderer
    {
    public:
        DepthBufferRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };
}

#endif // RENDERER_H