#ifndef OBJECT_H
#define OBJECT_H

#include "rtmath.h"
#include "object.h"
#include "ray.h"
#include "bvh.h"
#include "random.h"
#include "material.h"

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace RenderToy
{
    class Mesh;

    class Geometry
    {
    protected:
        Matrix4x4f object_to_world;
        Matrix4x4f world_to_object;

    public:
        std::string name;
        /// @brief Construct a Geometry Object with a default O2W matrix.
        Geometry();

        /// @brief Set object to world matrix.
        /// @param object_to_world O2W Matrix, should be an AFFINE matrix.
        virtual void SetO2W(const Matrix4x4f &object_to_world_);

        /// @brief Get O2W matrix of the Geometry Object.
        /// @return
        const Matrix4x4f &GetO2W() const;
        /// @brief Get W2O matrix of the Geometry Object.
        /// @return
        const Matrix4x4f &GetW2O() const;

        const Vector3f O2WTransform(const Vector3f &vec) const;
        const Vector3f W2OTransform(const Vector3f &vec) const;

        const Ray O2WTransform(const Ray &ray) const;
        const Ray W2OTransform(const Ray &ray) const;
    };

    class Camera : public Geometry
    {
    public:
        float focal_length;      // in mm
        Vector2f gate_dimension; // in inches
        float near_clipping_plane;
        float far_clipping_plane;

        Camera(Matrix4x4f object_to_world_,
               float focal_length_,
               Vector2f gate_dimension_,
               float near_clipping_plane_,
               float far_clipping_plane_);
    };

    static const Camera academy_camera = Camera(Matrix4x4f::I, 35.0f, Vector2f(0.825f, 0.446f), 0.1f, 1000.0f);

    /// @brief Triangle is the basic discrete element of every mesh in the scene. It directly composites BVH.
    class Triangle
    {
    public:
        /// @brief Get the bounding box of the triangle.
        /// @return Bounding box of the triangle.
        const BoundingBox BBox() const;

        std::array<Vector3f, 3> vert;
        std::array<Vector3f, 3> vert_w;
        std::array<Vector3f, 3> norm;
        std::array<Vector2f, 3> uv;

        Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_);
        /// @brief Do ray-triangle intersection test in WORLD SPACE.
        /// @param ray Incoming ray.
        /// @param t
        /// @param u
        /// @param v
        /// @return
        const bool Intersect(const Ray &ray, float &t, float &u, float &v) const;
        /// @brief Get sample point in WORLD SPACE.
        /// @param random
        /// @return
        const Vector3f GetSamplePoint() const;
        /// @brief Get cached area.
        /// @return
        const float AreaC() const;
        /// @brief Get cached tangent.
        /// @return
        const Vector3f TangentC() const;
        /// @brief Get cached normal.
        /// @return
        const Vector3f NormalC() const;

        /// @brief Update cache. Should be evaluated after O2W matrix having been changed.
        void UpdateCache();

        Mesh *parent;

    private:
        const Vector3f Tangent() const;
        const Vector3f Normal() const;
        const float Area() const;

        Vector3f v0v1_w;
        Vector3f v0v2_w;

        float area;
        Vector3f normal;
        Vector3f tangent;
    };

    class Mesh : public Geometry
    {
    public:
        std::vector<Triangle *> faces;
        PrincipledBSDF *tex;
        bool smooth{false};

        /// @brief Set object to world matrix.
        /// @param object_to_world O2W Matrix, should be an AFFINE matrix.
        virtual void SetO2W(const Matrix4x4f &object_to_world_) override;
    };
};

#endif // OBJECT_H