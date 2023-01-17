#ifndef WORLD_H
#define WORLD_H

#include "meshobj.h"
#include "camera.h"

#include <vector>

namespace OpenPT
{
    struct World
    {
        std::vector<Mesh> meshes;
        std::vector<Camera> cameras;
    };
}

#endif // WORLD_H