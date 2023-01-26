#ifndef GEOOBJ_H
#define GEOOBJ_H

#include "mathfunc.h"
#include "ray.h"

namespace RenderToy
{
    class GeoObj
    {
    protected:
        Matrix4x4f object_to_world;
        Matrix4x4f world_to_object;

    public:
        std::string name;
        GeoObj();
        
        /// @brief Set object to world matrix.
        /// @param object_to_world O2W Matrix, should be an AFFINE matrix.
        virtual void SetO2W(const Matrix4x4f &object_to_world_);
        const Matrix4x4f &GetO2W() const;
        const Matrix4x4f &GetW2O() const;

        const Vector3f O2WTransform(const Vector3f &vec) const;
        const Vector3f W2OTransform(const Vector3f &vec) const;

        const Ray O2WTransform(const Ray &ray) const;
        const Ray W2OTransform(const Ray &ray) const;
    };
}

#endif // GEOOBJ_H