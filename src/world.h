#ifndef WORLD_H
#define WORLD_H

#include "meshobj.h"
#include "camera.h"

#include <vector>

namespace OpenPT
{
    struct World
    {
        std::vector<Mesh *> meshes;
        std::vector<const Triangle *> triangles;
        std::vector<Camera> cameras;

        std::vector<Mesh *> emissive_meshes;
        
        /// @brief Randomly chooses a emissive triangle in the scene. Used by source sampling.
        /// @param random 
        /// @param position_o 
        /// @param id_o 
        void SampleEmitter(const Random &random, Vector3f &position_o, const Triangle *&id_o) const;

        int CountEmitters() const;
    };
}

#endif // WORLD_H