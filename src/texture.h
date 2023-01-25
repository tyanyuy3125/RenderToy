#ifndef TEXTURE_H
#define TEXTURE_H

#include "mathfunc.h"

namespace OpenPT
{
    enum class MaterialType
    {
        DIFFUSE,
        MICROFACET_DIFFUSE,
        MICROFACET_GLOSSY
    };

    struct Material
    {
        MaterialType type;
        Vector3f kd;
        Vector3f emitivity;
        Material();
        Material(MaterialType type_, const Vector3f &kd_, const Vector3f &emitivity_);

        const float PDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
    private:
        static const Vector3f EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N);

    };
}

#endif // TEXTURE_H