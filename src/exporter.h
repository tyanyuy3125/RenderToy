#ifndef EXPORTER_H
#define EXPORTER_H

#include "mathfunc.h"
#include "renderer.h"

#include <ostream>

namespace OpenPT
{
    struct IExporter
    {
        RenderContext *render_context;
        virtual void Export(std::ostream &os) = 0;

        IExporter(RenderContext *render_context_);
    };

    struct PPMExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        PPMExporter(RenderContext *render_context_);
    };

    struct BMPExporter : public IExporter
    {
        virtual void Export(std::ostream &os) override final;

        BMPExporter(RenderContext *render_context_);
    };
}

#endif // EXPORTER_H