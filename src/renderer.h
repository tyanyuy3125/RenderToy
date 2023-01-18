#ifndef RENDERER_H
#define RENDERER_H

#include <ostream>

#include "world.h"
#include "mathfunc.h"

namespace OpenPT
{
    // Use Blender conventions.
    struct FormatSettings
    {
        Size resolution = Size(1920, 1080);
        Vector2f aspect = Vector2f(16.0f, 9.0f);

        FormatSettings() = default;
        FormatSettings(Size resolution_, Vector2f aspect_);
    };

    class IRenderer
    {
    private:
        World *world;

    public:
        FormatSettings format_settings;

        IRenderer() = delete;
        IRenderer(const IRenderer &) = delete;
        IRenderer(const IRenderer &&) = delete;
        IRenderer(World *world_);
        virtual void Render(int camera_id, Vector3f *&buffer) = 0;
    };

    class TestRenderer : public IRenderer
    {
    public:
        TestRenderer();
        virtual void Render(int camera_id, Vector3f *&buffer) override final;
    };

    class IntersectTestRenderer : public IRenderer
    {
    public:
        IntersectTestRenderer(World *world_);
        virtual void Render(int camera_id, Vector3f *&buffer) override final;
    };
}

#endif // RENDERER_H