#include "geoobj.h"

namespace OpenPT
{
    GeoObj::GeoObj()
        : object_to_world(Matrix4x4f::I), world_to_object(Matrix4x4f::I)
    {
    }

    void GeoObj::SetO2W(const Matrix4x4f &object_to_world_)
    {
        object_to_world = object_to_world_;
        Matrix3x3f R = object_to_world.ComplementMinor(3, 3);
        Vector3f C = {object_to_world[0][3], object_to_world[1][3], object_to_world[2][3]};
        R.Transpose();
        C = (-1.0f) * (R * C);
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                world_to_object[i][j] = R[i][j];
            }
            world_to_object[i][3] = C[i];
        }
        world_to_object[3][3] = 1.0f;
    }

    const Matrix4x4f &GeoObj::GetO2W() const
    {
        return object_to_world;
    }

    const Matrix4x4f &GeoObj::GetW2O() const
    {
        return world_to_object;
    }

    const Vector3f GeoObj::O2WTransform(const Vector3f &vec) const
    {
        Vector4f vec4(vec, 1.0f);
        vec4 = object_to_world * vec4;
        return Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Vector3f GeoObj::W2OTransform(const Vector3f &vec) const
    {
        Vector4f vec4(vec, 1.0f);
        vec4 = world_to_object * vec4;
        return Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Ray GeoObj::O2WTransform(const Ray &ray) const
    {
        Ray ret(O2WTransform(ray.src), O2WTransform(ray.direction));
        ret.direction -= ret.src;
        ret.direction.Normalize();
        return ret;
    }

    const Ray GeoObj::W2OTransform(const Ray &ray) const
    {
        Ray ret(W2OTransform(ray.src), W2OTransform(ray.direction));
        ret.direction -= ret.src;
        ret.direction.Normalize();
        return ret;
    }
}