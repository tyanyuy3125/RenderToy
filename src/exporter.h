#ifndef EXPORTER_H
#define EXPORTER_H

#include "mathfunc.h"
#include "renderer.h"

#include <ostream>

namespace OpenPT
{
    struct IExporter
    {
        FormatSettings format_settings;
        virtual void Export(std::ostream &os, Vector3f *buffer) = 0;

        IExporter();
        IExporter(FormatSettings format_settings_);
    };

    struct PPMExporter : public IExporter
    {
        virtual void Export(std::ostream &os, Vector3f *buffer) override final;

        PPMExporter();
        PPMExporter(FormatSettings format_settings_);
    };

    struct BMPExporter : public IExporter
    {
        virtual void Export(std::ostream &os, Vector3f *buffer) override final;

        BMPExporter();
        BMPExporter(FormatSettings format_settings_);
    };
}

#endif // EXPORTER_H