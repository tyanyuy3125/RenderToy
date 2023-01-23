#ifndef RENDERER_H
#define RENDERER_H

#include <ostream>

#include "world.h"
#include "mathfunc.h"
#include "bvh.h"
#include "camera.h"

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
        void PrepareScreenSpace(Camera *cam, float &top, float &right);

    public:
        RenderContext *render_context;
        IRenderer() = delete;
        IRenderer(const IRenderer &) = delete;
        IRenderer(const IRenderer &&) = delete;
        IRenderer(RenderContext *render_context_);
        virtual void Render() = 0;
    };

    class TestRenderer : public IRenderer
    {
    public:
        TestRenderer(const Size &resolution_);
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
        float near, far;

        DepthBufferRenderer(RenderContext *render_context_);
        DepthBufferRenderer(RenderContext *render_context_, float near_, float far_);
        virtual void Render() override final;
    };

    class NormalRenderer : public IRenderer
    {
    public:
        NormalRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };

    class PathTracingRenderer : public IRenderer
    {
    public:
        PathTracingRenderer(RenderContext *render_context_);
        virtual void Render() override final;

    private:
        const Vector3f Radiance(const Vector3f &ray_src, const Vector3f &ray_dir, const Random &random, const Triangle *last_hit) const;
        const Vector3f SampleEmitters(const Vector3f &ray_dir, const SurfacePoint &surface_point, const Random &random) const;
    };
}

#endif // RENDERER_H