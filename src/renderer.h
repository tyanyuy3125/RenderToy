#ifndef RENDERER_H
#define RENDERER_H

#include <ostream>

#include "world.h"
#include "rtmath.h"
#include "bvh.h"
#include "surfacepoint.h"
#include "random.h"

namespace RenderToy
{
    // Use Blender conventions.
    struct FormatSettings
    {
        SizeN resolution = SizeN(1920, 1080);
        Vector2f aspect = Vector2f(1.0f, 1.0f); // Don't touch it!

        FormatSettings() = default;
        FormatSettings(SizeN resolution_, Vector2f aspect_);
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
        TestRenderer(const SizeN &resolution_);
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
        int iteration_count;

    public:
        PathTracingRenderer(RenderContext *render_context_, const int iteration_count_);
        virtual void Render() override final;

    private:
        const Vector3f Radiance(const Ray &cast_ray, const Triangle *last_hit, RayState &state, const int depth, const float last_bsdfpdf) const;
        const Vector3f DirectLight(const RayState state, const Vector3f &ray_dir, const SurfacePoint &surface_point) const;
    };

    class AlbedoRenderer : public IRenderer
    {
    public:
        AlbedoRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };
}

#endif // RENDERER_H