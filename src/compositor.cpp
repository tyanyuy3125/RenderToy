#include "compositor.h"



const Image RenderToy::Image::GaussianBlur(const int diameter) const
{
    Image ret(this->resolution);

    return ret;
}

RenderToy::Image::Image(const Size &resolution_)
    : resolution(resolution_)
{
    buffer = new Vector3f[resolution.Area()];
}

RenderToy::Image::Image(const RenderContext *const render_context_)
    : resolution(render_context_->format_settings.resolution)
{
    buffer = new Vector3f[resolution.Area()];
    for(int i=0;i<resolution.Area();++i)
    {
        buffer[i] = render_context_->buffer[i];
    }
}

RenderToy::Image::Image(const Image &image)
    : resolution(image.resolution)
{
    buffer = new Vector3f[resolution.Area()];
    for(int i=0;i<resolution.Area();++i)
    {
        buffer[i] = image.buffer[i];
    }
}

RenderToy::Image::~Image()
{
    delete[] buffer;
}
