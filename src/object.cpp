#include <RenderToy/object.h>

#include <limits>
#include <cmath>
#include <iostream>

namespace RenderToy
{
    Geometry::Geometry()
        : object_to_world(Matrix4x4f::I), world_to_object(Matrix4x4f::I)
    {
    }

    void Geometry::SetO2W(const Matrix4x4f &object_to_world_)
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

    const Matrix4x4f &Geometry::GetO2W() const
    {
        return object_to_world;
    }

    const Matrix4x4f &Geometry::GetW2O() const
    {
        return world_to_object;
    }

    const Vector3f Geometry::O2WTransform(const Vector3f &vec) const
    {
        // Vector4f vec4(vec, 1.0f);
        Vector4f vec4(vec.x(), vec.y(), vec.z(), 1.0f);
        vec4 = object_to_world * vec4;
        return Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Vector3f Geometry::W2OTransform(const Vector3f &vec) const
    {
        // Vector4f vec4(vec, 1.0f);
        Vector4f vec4(vec.x(), vec.y(), vec.z(), 1.0f);
        vec4 = world_to_object * vec4;
        return Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Ray Geometry::O2WTransform(const Ray &ray) const
    {
        Ray ret(O2WTransform(ray.src), O2WTransform(ray.direction));
        ret.direction -= ret.src;
        ret.direction.Normalize();
        return ret;
    }

    const Ray Geometry::W2OTransform(const Ray &ray) const
    {
        Ray ret(W2OTransform(ray.src), W2OTransform(ray.direction));
        ret.direction -= ret.src;
        ret.direction.Normalize();
        return ret;
    }

    Camera::Camera(Matrix4x4f object_to_world_, float focal_length_, Vector2f gate_dimension_, float near_clipping_plane_, float far_clipping_plane_)
        : focal_length(focal_length_),
          gate_dimension(gate_dimension_),
          near_clipping_plane(near_clipping_plane_),
          far_clipping_plane(far_clipping_plane_)
    {
        Geometry::SetO2W(object_to_world);
    }

    const BoundingBox Triangle::BBox() const
    {
        Vector3f v0 = vert_w[0];
        Vector3f v1 = vert_w[1];
        Vector3f v2 = vert_w[2];

        Vector3f bmax = {
            std::max(v0.x(), std::max(v1.x(), v2.x())),
            std::max(v0.y(), std::max(v1.y(), v2.y())),
            std::max(v0.z(), std::max(v1.z(), v2.z()))};
        Vector3f bmin = {
            std::min(v0.x(), std::min(v1.x(), v2.x())),
            std::min(v0.y(), std::min(v1.y(), v2.y())),
            std::min(v0.z(), std::min(v1.z(), v2.z()))};

        return BoundingBox(bmin, bmax);
    }

    Triangle::Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_)
        : vert(vert_), vert_w(vert_), norm(norm_), uv(uv_), parent(parent_)
    {
        UpdateCache();
    }

    const bool Triangle::Intersect(const Ray &ray, float &t, float &u, float &v) const
    {
        Vector3f pvec = ray.direction.Cross(v0v2_w);
        float det = v0v1_w.Dot(pvec);

        if (det > 0.0f)
        {
            Vector3f tvec = ray.src - vert_w[0];
            auto alpha = pvec.Dot(tvec);
            if (alpha < 0.0f || alpha > det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1_w);
            auto beta = qvec.Dot(ray.direction);
            if (beta < 0.0f || alpha + beta > det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2_w.Dot(qvec) * inv_det;
            if (t < 0.0f)
            {
                return false;
            }
            u = alpha * inv_det;
            v = beta * inv_det;

            return true;
        }
        else if (det < 0.0f)
        {
            Vector3f tvec = ray.src - vert_w[0];
            auto alpha = pvec.Dot(tvec);
            if (alpha > 0.0f || alpha < det)
            {
                return false;
            }

            Vector3f qvec = tvec.Cross(v0v1_w);
            auto beta = qvec.Dot(ray.direction);
            if (beta > 0.0f || alpha + beta < det)
            {
                return false;
            }

            float inv_det = 1.0f / det;
            t = v0v2_w.Dot(qvec) * inv_det;
            if (t < 0.0f)
            {
                return false;
            }
            u = alpha * inv_det;
            v = beta * inv_det;

            return true;
        }
        return false;
    }

    const Vector3f Triangle::GetSamplePoint() const
    {
        float sqr1 = std::sqrt(Random::Float());
        float r2 = Random::Float();

        float a = 1.0f - sqr1;
        float b = (1.0f - r2) * sqr1;

        return v0v1_w * a + v0v2_w * b + vert_w[0];
    }

    const Vector3f Triangle::Tangent() const
    {
        return v0v1_w.Normalized();
    }

    const Vector3f Triangle::Normal() const
    {
        Vector3f v1 = vert_w[1];
        Vector3f v2 = vert_w[2];
        return Tangent().Cross(v2 - v1).Normalized();
    }

    const float Triangle::Area() const
    {
        return v0v1_w.Cross(v0v2_w).Length() * 0.5f;
    }

    const float Triangle::AreaC() const
    {
        return area;
    }

    const Vector3f Triangle::TangentC() const
    {
        return tangent;
    }

    const Vector3f Triangle::NormalC(const float u, const float v) const
    {
        if (parent->smooth)
        {
            return ((1 - u - v) * norm[0] + u * norm[1] + v * norm[2]).Normalized();
        }
        else
        {
            return normal;
        }
    }

    const Vector3f Triangle::GeometricalNormalC() const
    {
        return normal;
    }

    void Triangle::UpdateCache()
    {
        if (parent != nullptr)
        {
            vert_w[0] = parent->O2WTransform(vert[0]);
            vert_w[1] = parent->O2WTransform(vert[1]);
            vert_w[2] = parent->O2WTransform(vert[2]);
        }
        else
        {
            vert_w[0] = vert[0];
            vert_w[1] = vert[1];
            vert_w[2] = vert[2];
        }
        v0v1_w = vert_w[1] - vert_w[0];
        v0v2_w = vert_w[2] - vert_w[0];
        area = Area();
        tangent = Tangent();
        normal = Normal();
    }

    const std::vector<float> Triangle::GetVBO(const std::vector<GLAttributeObject> &attrib_list) const
    {
        std::vector<float> ret;
        for (std::size_t i = 0; i < 3; ++i)
        {
            for (auto attrib : attrib_list)
            {
                switch (attrib)
                {
                case GLAttributeObject::kVert:
                    ret.push_back(vert[i][0]);
                    ret.push_back(vert[i][1]);
                    ret.push_back(vert[i][2]);
                    break;
                case GLAttributeObject::kNorm:
                    ret.push_back(norm[i][0]);
                    ret.push_back(norm[i][1]);
                    ret.push_back(norm[i][2]);
                    break;
                case GLAttributeObject::kUV:
                    ret.push_back(uv[i][0]);
                    ret.push_back(uv[i][1]);
                    break;
                }
            }
        }
        return ret;
    }

    const void Triangle::AppendVBO(std::vector<float> &target, const std::vector<GLAttributeObject> &attrib_list) const
    {
        for (std::size_t i = 0; i < 3; ++i)
        {
            for (auto attrib : attrib_list)
            {
                switch (attrib)
                {
                case GLAttributeObject::kVert:
                    target.push_back(vert[i][0]);
                    target.push_back(vert[i][1]);
                    target.push_back(vert[i][2]);
                    break;
                case GLAttributeObject::kNorm:
                    target.push_back(norm[i][0]);
                    target.push_back(norm[i][1]);
                    target.push_back(norm[i][2]);
                    break;
                case GLAttributeObject::kUV:
                    target.push_back(uv[i][0]);
                    target.push_back(uv[i][1]);
                    break;
                }
            }
        }
    }

    void Mesh::SetO2W(const Matrix4x4f &object_to_world_)
    {
        Geometry::SetO2W(object_to_world_);
        for (auto tri : tris)
        {
            tri->UpdateCache();
        }
    }

    const std::vector<float> Mesh::GetVBO(const std::vector<GLAttributeObject> &attrib_list) const
    {
        std::vector<float> ret;
        for (auto tri : tris)
        {
            tri->AppendVBO(ret, attrib_list);
        }
        return ret;
    }

    const void Mesh::AppendVBO(std::vector<float> &target, const std::vector<GLAttributeObject> &attrib_list) const
    {
        for (auto tri : tris)
        {
            tri->AppendVBO(target, attrib_list);
        }
    }

    Polygon::Polygon(const std::size_t V_, const std::vector<Vector3f> &vert_, const std::vector<Vector3f> &norm_, const std::vector<Vector2f> &uv_, Mesh *parent_)
        : V(V_), vert(vert_), norm(norm_), uv(uv_), parent(parent_)
    {
    }

    std::vector<Triangle *> Polygon::ConvertToTriangle() const
    {
        std::vector<Triangle *> ret;
        std::size_t index_a = 0;
        for (std::size_t index_b = 1, index_c = 2; index_c < V; ++index_b, ++index_c)
        {
            ret.push_back(new Triangle({vert[index_a], vert[index_b], vert[index_c]}, {norm[index_a], norm[index_b], norm[index_c]}, {uv[index_a], uv[index_b], uv[index_c]}, parent));
        }
        return ret;
    }

    Light::Light(Vector3f color_, float intensity_)
        : color(color_), intensity(intensity_)
    {
    }

    DeltaLight::DeltaLight(Vector3f color_, float intensity_)
        : Light(color_, intensity_)
    {
    }

    const Vector3f DeltaLight::GetSamplePoint() const
    {
        return {object_to_world[0][3], object_to_world[1][3], object_to_world[2][3]};
    }

    DirectionalLight::DirectionalLight(Vector3f color_, float intensity_)
        : Light(color_, intensity_)
    {
    }

    const Vector3f RenderToy::DirectionalLight::GetSamplePoint() const
    {
        return O2WTransform(kFloatInfinity * Vector3f(0.0f,0.0f,1.0f));
    }
}