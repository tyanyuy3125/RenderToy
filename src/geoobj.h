#ifndef GEOOBJ_H
#define GEOOBJ_H

#include "mathfunc.h"

namespace OpenPT
{
    class GeoObj
    {
    public:
        Matrix4x4f object_to_world;

        GeoObj(Matrix4x4f object_to_world_ = Matrix4x4f::I);
    };
}

#endif // GEOOBJ_H