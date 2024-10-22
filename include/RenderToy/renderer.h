#ifndef RENDERER_H
#define RENDERER_H

#include <ostream>

#include "world.h"
#include "rtmath.h"
#include "bvh.h"
#include "surfacepoint.h"

namespace RenderToy
{
    /// @brief Render technology adapted.
    enum class RenderTech
    {
        kTest = 0,
        kIntersectTest,
        kDepthBuffer,
        kNormal,
        kPathTracing,
        kAlbedo,
        kLineEdge
    };

    /// @brief Projection mode adapted.
    enum class ProjectionMode
    {
        kPerspective = 0,
        kParallel
    };

    // Use Blender conventions.
    /// @brief Format settings for the renderer.
    struct FormatSettings
    {
        SizeN resolution = SizeN(1920, 1080);
        Vector2f aspect = Vector2f(1.0f, 1.0f); // Don't touch it!

        FormatSettings() = default;
        FormatSettings(SizeN resolution_, Vector2f aspect_);
    };

    /// @brief Render context.
    struct RenderContext
    {
        World *world;
        BVH *bvh;

        Vector3f *buffer;

        FormatSettings format_settings;
        int camera_id = 0;

        /// @brief Initialize a render context with world pointer and format settings.
        /// @param world_ 
        /// @param format_settings_ 
        RenderContext(World *world_, FormatSettings format_settings_);
        ~RenderContext();

        Vector3f &operator()(const std::size_t x, const std::size_t y);
        const Vector3f &operator()(const std::size_t x, const std::size_t y) const;
    };

    /// @brief Base class of renderers.
    class Renderer
    {
    protected:
        void PrepareScreenSpace(Camera *cam, float &top, float &right);

    public:
        RenderContext *render_context;
        Renderer() = delete;
        Renderer(const Renderer &) = delete;
        Renderer(const Renderer &&) = delete;
        Renderer(RenderContext *render_context_);
        /// @brief Render an image.
        virtual void Render() = 0;
    };

    /// @brief Test renderer.
    class TestRenderer : public Renderer
    {
    public:
        TestRenderer(const SizeN &resolution_);
        virtual void Render() override final;
    };

    /// @brief Renderer for intersection algorithm test.
    class IntersectTestRenderer : public Renderer
    {
    public:
        IntersectTestRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };

    /// @brief Depth buffer renderer.
    class DepthBufferRenderer : public Renderer
    {
    public:
        float near, far;

        DepthBufferRenderer(RenderContext *render_context_);
        DepthBufferRenderer(RenderContext *render_context_, float near_, float far_);
        virtual void Render() override final;
    };

    /// @brief Normal pass renderer.
    class NormalRenderer : public Renderer
    {
    public:
        NormalRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };

    /// @brief Path tracing renderer.
    class PathTracingRenderer : public Renderer
    {
        int iteration_count;

    public:
        PathTracingRenderer(RenderContext *render_context_, const int iteration_count_);
        virtual void Render() override final;

    private:
        const Vector3f Radiance(const Ray &cast_ray, const Triangle *last_hit, RayState &state, const int depth, const float last_bsdfpdf) const;
        const Vector3f DirectLight(const RayState state, const Vector3f &ray_dir, const SurfacePoint &surface_point) const;
    };

    /// @brief Albedo pass renderer.
    class AlbedoRenderer : public Renderer
    {
    public:
        AlbedoRenderer(RenderContext *render_context_);
        virtual void Render() override final;
    };
}

#endif // RENDERER_H