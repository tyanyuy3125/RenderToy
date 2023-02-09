#ifndef EXPORTER_H
#define EXPORTER_H

#include "rtmath.h"
#include "renderer.h"
#include "compositor.h"

#include <ostream>

namespace RenderToy
{
    /// @brief Interface for all exporters.
    struct IExporter
    {
        const Image &image;
        virtual void Export(std::ostream &os) = 0;

        IExporter(const Image &image_);
    };

    /// @brief Exports image as .ppm file.
    struct PPMExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        PPMExporter(const Image &image_);
    };

    /// @brief Exports image as .bmp file.
    struct BMPExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        BMPExporter(const Image &image_);
    };

    /// @brief Naive ASCII Art exporter.
    struct ASCIIExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        ASCIIExporter(const Image &image_);

    };
}

#endif // EXPORTER_H