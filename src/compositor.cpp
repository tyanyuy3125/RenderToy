#include <RenderToy/compositor.h>
#include <RenderToy/exception.h>

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

Vector3f &RenderToy::Image::operator()(const std::size_t x, const std::size_t y)
{
    return buffer[y * resolution.width + x];
}

const Vector3f &RenderToy::Image::operator()(const std::size_t x, const std::size_t y) const
{
    return buffer[y * resolution.width + x];
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

const void RenderToy::Image::Transform(const std::function<void(Vector3f &)> &pixel_processor)
{
    for (int i = 0; i < resolution.Area(); ++i)
    {
        pixel_processor(buffer[i]);
    }
}

Image &RenderToy::Image::Bloom(const std::size_t size, const float sigma, const float threshold)
{
    Image bloom_layer = Extract([&threshold](const Vector3f &_) -> bool
                                { return (Convert::Luma(_) > threshold); });
    bloom_layer.GaussianBlur(size, sigma);
    (*this) = (*this) + bloom_layer;

    return (*this);
}

Vector3f *&RenderToy::Image::GetBuffer()
{
    return buffer;
}

const Vector3f *const &RenderToy::Image::GetBuffer() const
{
    return buffer;
}

RenderToy::Image::~Image()
{
    delete[] buffer;
}

Image &RenderToy::Image::GammaCorrection(const float gamma)
{
    auto ratio = 1.0f / gamma;
    Transform([&ratio](Vector3f &_) -> void
              { _ = Vector3f::Pow(_, ratio); });
    return (*this);
}

const Image Image::operator+(const Image &img)
{
    if (resolution != img.resolution)
    {
        throw Exception::ImageSizeNotMatchException("The + operator of two images requires two images have the same resolution.");
    }

    Image ret(resolution);
    for (int i = 0; i < resolution.Area(); ++i)
    {
        ret.buffer[i] = buffer[i] + img.buffer[i];
    }
    return ret;
}

const Image Image::operator-(const Image &img)
{
    if (resolution != img.resolution)
    {
        throw Exception::ImageSizeNotMatchException("The - operator of two images requires two images have the same resolution.");
    }

    Image ret(resolution);
    for (int i = 0; i < resolution.Area(); ++i)
    {
        ret.buffer[i] = buffer[i] - img.buffer[i];
    }
    return ret;
}

const Image Image::operator*(const Image &img)
{
    if (resolution != img.resolution)
    {
        throw Exception::ImageSizeNotMatchException("The * operator of two images requires two images have the same resolution.");
    }

    Image ret(resolution);
    for (int i = 0; i < resolution.Area(); ++i)
    {
        ret.buffer[i] = buffer[i] * img.buffer[i];
    }
    return ret;
}

const Image Image::operator/(const Image &img)
{
    if (resolution != img.resolution)
    {
        throw Exception::ImageSizeNotMatchException("The / operator of two images requires two images have the same resolution.");
    }

    Image ret(resolution);
    for (int i = 0; i < resolution.Area(); ++i)
    {
        ret.buffer[i] = buffer[i] / img.buffer[i];
    }
    return ret;
}

const void Image::operator=(const Image &img)
{
    // resolution = img.resolution;
    if (resolution != img.resolution)
    {
        throw Exception::ImageSizeNotMatchException("The = operator of two images requires two images have the same resolution.");
    }

    for (int i = 0; i < resolution.Area(); ++i)
    {
        buffer[i] = img.buffer[i];
    }
}

void Image::DrawLine(PointN p1, PointN p2, const Vector3f &color)
{
    p1.SizeClamp(resolution);
    p2.SizeClamp(resolution);

    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    if (std::abs(dy) < std::abs(dx))
    {
        if (p1.x > p2.x)
        {
            std::swap(p1, p2);
        }
        int dy = p2.y - p1.y;
        bool dy_positive = true;
        if (dy < 0)
        {
            dy_positive = false;
            dy = -dy;
        }
        int dx = p2.x - p1.x;
        int n_err = 0;
        int current_y = p1.y;
        for (int i = p1.x; i < p2.x; ++i)
        {
            if (n_err > dx)
            {
                current_y += (dy_positive ? 1 : -1);
                n_err -= 2 * dx;
            }
            buffer[current_y * resolution.width + i] = color;
            n_err += 2 * dy;
        }
    }
    else
    {
        if (p1.y > p2.y)
        {
            std::swap(p1, p2);
        }
        int dx = p2.x - p1.x;
        bool dx_positive = true;
        if (dx < 0)
        {
            dx_positive = false;
            dx = -dx;
        }
        int dy = p2.y - p1.y;
        int n_err = 0;
        int current_x = p1.x;
        for (int i = p1.y; i < p2.y; ++i)
        {
            if (n_err > dy)
            {
                current_x += (dx_positive ? 1 : -1);
                n_err -= 2 * dy;
            }
            buffer[i * resolution.width + current_x] = color;
            n_err += 2 * dx;
        }
    }
}

void Image::Fill(PointN p1, PointN p2, const Vector3f &color)
{
    p1.SizeClamp(resolution);
    p2.SizeClamp(resolution);

    if (p1.x > p2.x)
    {
        std::swap(p1.x, p2.x);
    }
    if (p1.y > p2.y)
    {
        std::swap(p1.y, p2.y);
    }
    for (int i = p1.x; i < p2.x; ++i)
    {
        for (int j = p1.y; j < p2.y; ++j)
        {
            buffer[j * resolution.width + i] = color;
        }
    }
}
