#include "compositor.h"

using namespace RenderToy;

RenderToy::Image::Image(const SizeN &resolution_)
    : resolution(resolution_)
{
    buffer = new Vector3f[resolution.Area()];
}

RenderToy::Image::Image(const RenderContext *const render_context_)
    : resolution(render_context_->format_settings.resolution)
{
    buffer = new Vector3f[resolution.Area()];
    for (int i = 0; i < resolution.Area(); ++i)
    {
        buffer[i] = render_context_->buffer[i];
    }
}

RenderToy::Image::Image(const Image &image)
    : resolution(image.resolution)
{
    buffer = new Vector3f[resolution.Area()];
    for (int i = 0; i < resolution.Area(); ++i)
    {
        buffer[i] = image.buffer[i];
    }
}

Image &RenderToy::Image::GaussianBlur(const std::size_t size, const float sigma)
{
    // UnitizedGaussianKernel<float> gk(size, sigma);
    UnitizedGaussianKernel<float, Orientation::X> gk_x(size, sigma);
    UnitizedGaussianKernel<float, Orientation::Y> gk_y(size, sigma);
    Convolute(gk_x).Convolute(gk_y);
    return (*this);
}

const Image RenderToy::Image::Extract(const std::function<bool(const Vector3f &)> &pixel_filter, const Vector3f &default_color)
{
    Image ret(resolution);
    for (int i = 0; i < resolution.Area(); ++i)
    {
        ret.buffer[i] = pixel_filter(buffer[i]) ? buffer[i] : default_color;
    }
    return ret;
}

Image &RenderToy::Image::Bloom(const std::size_t size, const float sigma, const float threshold)
{
    Image bloom_layer = Extract([&threshold](const Vector3f &_) -> bool {
        return (Convert::Luma(_) > threshold);
    });
    bloom_layer.GaussianBlur(size, sigma);
    (*this) = (*this) + bloom_layer;

    return (*this);
}

RenderToy::Image::~Image()
{
    delete[] buffer;
}
