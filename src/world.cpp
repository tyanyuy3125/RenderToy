#include "world.h"

#include <cmath>

void OpenPT::World::SampleEmitter(const Random &random, Vector3f &position_o, const Triangle *&id_o) const
{
    if (!emissive_meshes.empty())
    {
        int index = static_cast<int>(std::floor(random.Float() *
                                                static_cast<float>(emissive_meshes.size())));
        index = index < emissive_meshes.size() ? index : int(emissive_meshes.size() - 1);

        auto selected_mesh = emissive_meshes[index];

        index = static_cast<int>(std::floor(random.Float() *
                                            static_cast<float>(selected_mesh->faces.size())));
        index = index < selected_mesh->faces.size() ? index : int(selected_mesh->faces.size() - 1);

        position_o = selected_mesh->faces[index]->GetSamplePoint(random);
        id_o = selected_mesh->faces[index];
    }
    else
    {
        position_o = Vector3f::O;
        id_o = nullptr;
    }
}
