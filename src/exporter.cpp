#define BUFFER(x, y, width) render_context->buffer[y * width + x]

#include <algorithm>

#include "mathfunc.h"
#include "exporter.h"

namespace OpenPT
{
    void PPMExporter::Export(std::ostream &os)
    {
        int nx = render_context->format_settings.resolution.width;
        int ny = render_context->format_settings.resolution.height;
        os << "P3" << std::endl
           << nx << ' ' << ny << std::endl
           << "255" << std::endl;
        for (int i = 0; i < ny; ++i)
        {
            for (int j = 0; j < nx; ++j)
            {
                auto real_color = BUFFER(j, i, nx);
                float r = std::clamp(real_color.x, 0.0f, 1.0f);
                float g = std::clamp(real_color.y, 0.0f, 1.0f);
                float b = std::clamp(real_color.z, 0.0f, 1.0f);
                int ir = int(255.99f * r);
                int ig = int(255.99f * g);
                int ib = int(255.99f * b);
                os << ir << ' ' << ig << ' ' << ib << '\n';
            }
        }
    }

    PPMExporter::PPMExporter(RenderContext *render_context_)
        : IExporter(render_context_)
    {
    }

    void BMPExporter::Export(std::ostream &os)
    {
#pragma pack(push, 1)
        struct BmpHeader
        {
            char bitmapSignatureBytes[2] = {'B', 'M'};
            uint32_t sizeOfBitmapFile;
            uint32_t reservedBytes = 0;
            uint32_t pixelDataOffset = 54;
        } bmpHeader;

        struct BmpInfoHeader
        {
            uint32_t sizeOfThisHeader = 40;
            int32_t width;
            int32_t height;
            uint16_t numberOfColorPlanes = 1;
            uint16_t colorDepth = 24;
            uint32_t compressionMethod = 0;
            uint32_t rawBitmapDataSize = 0; // Ignore
            int32_t horizontalResolution = 3780;
            int32_t verticalResolution = 3780;
            uint32_t colorTableEntries = 0;
            uint32_t importantColors = 0;
        } bmpInfoHeader;

        struct Pixel
        {
            uint8_t blue;
            uint8_t green;
            uint8_t red;
        } pixel;
#pragma pack(pop)

        int padding_len = 4 - (render_context->format_settings.resolution.width % 4);
        padding_len %= 4;

        bmpHeader.sizeOfBitmapFile = 54 + render_context->format_settings.resolution.Area() * 3;
        bmpInfoHeader.width = render_context->format_settings.resolution.width;
        bmpInfoHeader.height = render_context->format_settings.resolution.height;

        os.write((char *)&bmpHeader, 14);
        os.write((char *)&bmpInfoHeader, 40);

        for (int i = 0; i < render_context->format_settings.resolution.height; ++i)
        {
            for (int j = 0; j < render_context->format_settings.resolution.width; ++j)
            {
                auto real_color = BUFFER(j, i, render_context->format_settings.resolution.width);
                float r = std::clamp(real_color.x, 0.0f, 1.0f);
                float g = std::clamp(real_color.y, 0.0f, 1.0f);
                float b = std::clamp(real_color.z, 0.0f, 1.0f);
                pixel.red = int(255.99f * r);
                pixel.green = int(255.99f * g);
                pixel.blue = int(255.99f * b);
                os.write((char *)&pixel, 3);
            }
            os.write(0x00, padding_len);
        }
    }

    BMPExporter::BMPExporter(RenderContext *render_context_)
        : IExporter(render_context_)
    {
    }

    IExporter::IExporter(RenderContext *render_context_)
        : render_context(render_context_)
    {
    }
}