#include "world.h"

#include <cmath>

void OpenPT::World::SampleEmitter(Vector3f &position_o, const Triangle *&id_o) const
{
    if (!emissive_triangles.empty())
    {
        id_o = emissive_triangles[Random::Int(0, emissive_triangles.size()-1)];
        position_o = id_o->GetSamplePoint();
    }
    else
    {
        position_o = Vector3f::O;
        id_o = nullptr;
    }
}

int OpenPT::World::CountEmitters() const
{
    return emissive_triangles.size();
}

void OpenPT::World::PrepareDirectLightSampling()
{
    emissive_triangles.clear();
    for(auto m : meshes)
    {
        if(m->tex.emitivity!=Vector3f::O)
        {
            for(auto t : m->faces)
            {
                emissive_triangles.push_back(t);
            }
        }
    }
}

const OpenPT::Vector3f OpenPT::World::GetDefaultEmission(const OpenPT::Vector3f &back_dir) const
{
    return (back_dir[2] < 0.0f) ? sky_emission : ground_reflection;
}
