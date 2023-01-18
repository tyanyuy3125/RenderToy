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
    };

    struct PPMExporter : public IExporter
    {
        virtual void Export(std::ostream &os, Vector3f *buffer) override final;
    };
}

#endif // EXPORTER_H