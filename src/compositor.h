#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "renderer.h"
#include "rtmath.h"

namespace RenderToy
{
    class Image
    {
        Vector3f *buffer;
        const Size resolution;

    public:
        Image(const Size &resolution_);
        Image(const RenderContext *const render_context_);
        Image(const Image &image);

        const Image GaussianBlur(const int diameter) const;

        ~Image();
    };
}

#endif // COMPOSITOR_H