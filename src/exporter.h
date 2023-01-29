#ifndef EXPORTER_H
#define EXPORTER_H

#include "mathfunc.h"
#include "renderer.h"

#include <ostream>

namespace RenderToy
{
    /// @brief Interface for all exporters.
    struct IExporter
    {
        RenderContext *render_context;
        virtual void Export(std::ostream &os) = 0;

        IExporter(RenderContext *render_context_);
    };

    /// @brief Exports image as .ppm file.
    struct PPMExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        PPMExporter(RenderContext *render_context_);
    };

    /// @brief Exports image as .bmp file.
    struct BMPExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        BMPExporter(RenderContext *render_context_);
    };
}

#endif // EXPORTER_H