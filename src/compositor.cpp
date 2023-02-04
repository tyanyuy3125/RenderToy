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

const Image RenderToy::Image::Convolute(const IConvolutionKernel<float> &k) const
{
    Image ret(resolution);
    size_t row_2 = k.kernel_mat.row >> 1;
    size_t column_2 = k.kernel_mat.column >> 1;
    for(int i=0;i<resolution.height;++i)
    {
        for(int j=0;j<resolution.width;++j)
        {
            //ret[i][j]
            for(int ii=0;ii<k.kernel_mat.row;++ii)
            {
                for(int jj=0;jj<k.kernel_mat.column;++jj)
                {
                    PointN buffer_pos = IndexWrapper<IndexWrapperType::kReflect>(PointN(i-row_2+ii, j-column_2+jj));
                    ret.buffer[i * ret.resolution.width + j] += buffer[buffer_pos.x * ret.resolution.width + buffer_pos.y] * k.kernel_mat[ii][jj];
                }
            }
        }
    }
    return ret;
}

const Image RenderToy::Image::GaussianBlur(const std::size_t size, const float sigma) const
{
    UnitizedGaussianKernel<float> gk(size, sigma);
    return Convolute(gk);
}

RenderToy::Image::~Image()
{
    delete[] buffer;
}
