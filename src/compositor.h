#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include "renderer.h"
#include "rtmath.h"

namespace RenderToy
{
    class Image
    {

        enum class IndexWrapperType
        {
            kReflect = 0
        };

        template <IndexWrapperType _IWT>
        inline const PointN IndexWrapper(PointN p) const
        {
            if constexpr (_IWT == IndexWrapperType::kReflect)
            {
                if (p.x < 0)
                    p.x = -p.x;
                if (p.y < 0)
                    p.y = -p.y;
                if (p.y >= resolution.width)
                    p.y = 2 * resolution.width - p.y;
                if (p.x >= resolution.height)
                    p.x = 2 * resolution.height - p.x;
                return p;
            }
            static_assert("No viable wrapper type specified.");
        }

    public:
        const SizeN resolution;
        Vector3f *buffer;

        Image(const SizeN &resolution_);
        Image(const RenderContext *const render_context_);
        Image(const Image &image);

        const Image Convolute(const IConvolutionKernel<float> &k) const;
        const Image GaussianBlur(const std::size_t size, const float sigma) const;

        ~Image();
    };
}

#endif // COMPOSITOR_H