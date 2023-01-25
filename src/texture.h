#ifndef TEXTURE_H
#define TEXTURE_H

#include "mathfunc.h"

namespace OpenPT
{
    struct Texture
    {
        Vector3f kd;
        Vector3f emitivity;
        Texture();
        Texture(const Vector3f &kd_, const Vector3f &emitivity_);
    };
}

#endif // TEXTURE_H