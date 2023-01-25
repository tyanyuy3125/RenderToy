#include "texture.h"
namespace OpenPT
{

    OpenPT::Texture::Texture()
        : reflectivity(Vector3f::O), emitivity(Vector3f::O)
    {
    }
    
    Texture::Texture(const Vector3f &reflectivity_, const Vector3f &emitivity_)
        : reflectivity(reflectivity_), emitivity(emitivity_)
    {
    }
}