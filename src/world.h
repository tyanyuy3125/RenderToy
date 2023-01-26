#ifndef WORLD_H
#define WORLD_H

#include "meshobj.h"
#include "camera.h"
#include "mathfunc.h"

#include <vector>

namespace RenderToy
{
    struct World
    {
        std::vector<Mesh *> meshes;
        std::vector<Triangle *> triangles;
        std::vector<Camera> cameras;

        /// @brief Randomly chooses a emissive triangle in the scene. Used by source sampling.
        /// @param random
        /// @param position_o
        /// @param id_o
        void SampleEmitter(Vector3f &position_o, const Triangle *&id_o) const;
        int CountEmitters() const;
        void PrepareDirectLightSampling();

        Vector3f sky_emission;
        Vector3f ground_reflection;

        const Vector3f GetDefaultEmission(const Vector3f &back_dir) const;

    private:
        std::vector<const Triangle *> emissive_triangles;
    };
}

#endif // WORLD_H