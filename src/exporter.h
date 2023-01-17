#ifndef EXPORTER_H
#define EXPORTER_H

#include "mathfunc.h"

#include <ostream>

namespace OpenPT
{
    void Export(std::ostream &os, Vector3f *buffer);
}

#endif // EXPORTER_H