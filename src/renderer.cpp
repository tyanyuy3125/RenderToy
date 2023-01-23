#define BUFFER(x, y, width) render_context->buffer[y * width + x]

#include "renderer.h"
#include "camera.h"
#include "ray.h"
#include "bvh.h"
#include "surfacepoint.h"

#include <algorithm>

namespace OpenPT
{
    IntersectTestRenderer::IntersectTestRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void IntersectTestRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);
        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::White;
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::O;
                }
            }
        }
    }

    TestRenderer::TestRenderer(const Size &resolution_)
        : IRenderer(new RenderContext(nullptr, FormatSettings(resolution_, Vector2f::O)))
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

    void IRenderer::PrepareScreenSpace(Camera *cam, float &top, float &right)
    {
        top = (Convert::InchToMM(cam->gate_dimension.y) / 2.0f) / cam->focal_length;
        right = (Convert::InchToMM(cam->gate_dimension.x) / 2.0f) / cam->focal_length;
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
    }

    IRenderer::IRenderer(RenderContext *render_context_)
        : render_context(render_context_)
    {
    }

    DepthBufferRenderer::DepthBufferRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
        near = render_context_->world->cameras[render_context_->camera_id].near_clipping_plane;
        far = render_context_->world->cameras[render_context_->camera_id].far_clipping_plane;
    }

    void DepthBufferRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_far_minus_near = 1.0f / (far - near);
        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(std::clamp((t - near) * div_far_minus_near, 0.0f, 1.0f));
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f(1.0f);
                }
            }
        }
    }

    DepthBufferRenderer::DepthBufferRenderer(RenderContext *render_context_, float near_, float far_)
        : IRenderer(render_context_), near(near_), far(far_)
    {
    }

    NormalRenderer::NormalRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void NormalRenderer::Render()
    {
        Camera *cam = &(render_context->world->cameras[render_context->camera_id]);
        float top, right;
        PrepareScreenSpace(cam, top, right);

        float div_resolution_width = 1.0f / float(render_context->format_settings.resolution.width);
        float div_resolution_height = 1.0f / float(render_context->format_settings.resolution.height);

        for (int y = 0; y < render_context->format_settings.resolution.height; ++y)
        {
            for (int x = 0; x < render_context->format_settings.resolution.width; ++x)
            {
                // (x, y) is the point in Raster Space.
                Vector2f NDC_coord = {float(x) * div_resolution_width, float(y) * div_resolution_height};
                Vector2f screen_coord = {2.0f * right * NDC_coord.x - right, 2.0f * top * NDC_coord.y - top};

                // Blender convention: Camera directing towards -z.
                Ray cast_ray(Vector3f::O, Vector3f(screen_coord, -1.0f));
                cast_ray = cam->O2WTransform(cast_ray);

                float t, u, v;
                auto intersected = render_context->bvh->Intersect(cast_ray, t, u, v);

                if (intersected != nullptr)
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = (((1 - u - v) * intersected->norm[0] + u * intersected->norm[1] + v * intersected->norm[2]) + Vector3f::White) / 2.0f;
                }
                else
                {
                    BUFFER(x, y, render_context->format_settings.resolution.width) = Vector3f::O;
                }
            }
        }
    }

    PathTracingRenderer::PathTracingRenderer(RenderContext *render_context_)
        : IRenderer(render_context_)
    {
    }

    void PathTracingRenderer::Render()
    {
    }

    const Vector3f PathTracingRenderer::Radiance(const Vector3f &ray_src, const Vector3f &ray_dir, const Random &random, const Triangle *last_hit) const
    {
        // intersect ray with scene
        const Triangle *pHitObject = nullptr;
        Vector3f hitPosition;
        Ray cast_ray(ray_src, ray_dir);
        pHitObject = render_context->bvh->Intersect(cast_ray, hitPosition); // TODO: last hit

        Vector3f radiance;
        if (pHitObject != nullptr)
        {
            // make surface point of intersection
            SurfacePoint surfacePoint(pHitObject, hitPosition);

            // local emission only for first-hit
            radiance = (last_hit != nullptr ? Vector3f::O : surfacePoint.GetEmission(ray_src, -ray_dir, false));

            // add emitter sample
            radiance = radiance + SampleEmitters(ray_dir, surfacePoint, random);

            // add recursive reflection
            //
            // single hemisphere sample, ideal diffuse BRDF:
            //    reflected = (inradiance * pi) * (cos(in) / pi * color) *
            //       reflectance
            // -- reflectance magnitude is 'scaled' by the russian roulette,
            // cos is importance sampled (both done by SurfacePoint),
            // and the pi and 1/pi cancel out -- leaving just:
            //    inradiance * reflectance color
            Vector3f nextDirection;
            Vector3f color;
            // check surface reflects ray
            if (surfacePoint.GetNextDirection(random, -ray_dir, nextDirection, color))
            {
                // recurse
                radiance = radiance + (color * Radiance(surfacePoint.GetPosition(),
                                                           nextDirection, random, surfacePoint.GetHitTriangle()));
            }
        }
        else
        {
            // no hit: default/background scene emission
            // radiance = pScene_m->getDefaultEmission(-rayDirection);
            radiance = Vector3f::O;
        }

        return radiance;
    }

    const Vector3f PathTracingRenderer::SampleEmitters(const Vector3f &ray_dir, const SurfacePoint &surface_point, const Random &random) const
    {
        Vector3f radiance;

        // single emitter sample, ideal diffuse BRDF:
        //    reflected = (emitivity * solidangle) * (emitterscount) *
        //       (cos(emitdirection) / pi * reflectivity)
        // -- SurfacePoint does the first and last parts (in separate methods)

        // get position on an emitter
        Vector3f emitterPosition;
        const Triangle *emitterId = nullptr;
        render_context->world->SampleEmitter(random, emitterPosition, emitterId);

        // check an emitter was found
        if (emitterId != nullptr)
        {
            // make direction to emit point
            const Vector3f emitDirection((emitterPosition - surface_point.GetPosition()).Normalized());

            // send shadow ray
            const Triangle *pHitObject = 0;
            Vector3f hitPosition;
            // TODO: Last hit.
            pHitObject = render_context->bvh->Intersect(Ray(surface_point.GetPosition(), emitDirection), hitPosition);

            // if unshadowed, get inward emission value
            Vector3f emissionIn;
            if ((pHitObject == nullptr) | (emitterId == pHitObject))
            {
                emissionIn = SurfacePoint(emitterId, emitterPosition).GetEmission(surface_point.GetPosition(), -emitDirection, true);
            }

            // get amount reflected by surface
            radiance = surface_point.GetReflection(emitDirection, emissionIn * static_cast<float>(render_context->world->CountEmitters()), -ray_dir);
        }

        return radiance;
    }
}
