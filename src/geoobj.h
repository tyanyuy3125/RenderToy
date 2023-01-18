#ifndef GEOOBJ_H
#define GEOOBJ_H

#include "mathfunc.h"

namespace OpenPT
{
    class GeoObj
    {
    protected:
        Matrix4x4f object_to_world;
        Matrix4x4f world_to_object;

    public:
        GeoObj();
        
        /// @brief Set object to world matrix.
        /// @param object_to_world O2W Matrix, should be an AFFINE matrix.
        void SetO2W(const Matrix4x4f &object_to_world_);
        const Matrix4x4f &GetO2W() const;
        const Matrix4x4f &GetW2O() const;
    };
}

#endif // GEOOBJ_H