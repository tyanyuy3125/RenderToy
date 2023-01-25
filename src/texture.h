#ifndef TEXTURE_H
#define TEXTURE_H

#include "mathfunc.h"

namespace OpenPT
{
    struct Texture
    {
        Vector3f kd;
        Vector3f ks = Vector3f::White;

        Vector3f emitivity;
        Texture();
        Texture(const Vector3f &kd_, const Vector3f &emitivity_);

        const Vector3f Eval(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;

    private:
        const Vector3f CookTorrance(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
        const Vector3f EvalDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &N) const;
    };
}

#endif // TEXTURE_H