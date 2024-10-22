#ifndef OBJECT_H
#define OBJECT_H

#include "rtmath.h"
#include "object.h"
#include "ray.h"
#include "bvh.h"
#include "material.h"

#include <vector>
#include <string>
#include <tuple>
#include <array>

namespace RenderToy
{
    class Mesh;

    /// @brief Parameter of OpenGL VAO for VBO exporting.
    enum class GLAttributeObject
    {
        kVert = 0,
        kNorm = 1,
        kUV = 2
    };

    /// @brief Fundamental geometry class. Storages the position and rotation information of objects in the world.
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

        /// @brief Transforms a vector by O2W matrix.
        /// @param vec
        /// @return
        const Vector3f O2WTransform(const Vector3f &vec) const;
        /// @brief Transforms a vector by W2O matrix.
        /// @param vec
        /// @return
        const Vector3f W2OTransform(const Vector3f &vec) const;
        /// @brief Transforms a ray by O2W matrix.
        /// @param ray
        /// @return
        const Ray O2WTransform(const Ray &ray) const;
        /// @brief Transforms a ray by W2O matrix.
        /// @param ray
        /// @return
        const Ray W2OTransform(const Ray &ray) const;
    };

    /// @brief Physically based camera.
    class Camera : public Geometry
    {
    public:
        float focal_length;      // in mm
        Vector2f gate_dimension; // in inches
        float near_clipping_plane;
        float far_clipping_plane;

        /// @brief Constructs Camera class.
        /// @param object_to_world_ O2W matrix.
        /// @param focal_length_ Focal length.
        /// @param gate_dimension_ Gate dimension.
        /// @param near_clipping_plane_ Near clipping plane, used for rasterizer (WIP) and Z-buffer.
        /// @param far_clipping_plane_ Far clipping plane, used for rasterizer (WIP) and Z-buffer.
        Camera(Matrix4x4f object_to_world_,
               float focal_length_,
               Vector2f gate_dimension_,
               float near_clipping_plane_,
               float far_clipping_plane_);
    };

    /// @brief Academy Format by AMPAS.
    static const Camera academy_format = Camera(Matrix4x4f::I, 35.0f, Vector2f(0.868f, 0.631f), 0.1f, 1000.0f);
    /// @brief IMAX HD by IMAX Corporation.
    static const Camera imax_hd = Camera(Matrix4x4f::I, 65.0f, Vector2f(2.772f, 2.072f), 0.1f, 1000.0f);
    /// @brief Maxivision by Dean Goodhill.
    static const Camera maxivision = Camera(Matrix4x4f::I, 35.0f, Vector2f(0.980f, 0.546f), 0.1f, 1000.0f);

    /// @brief Triangle is the basic discrete element of every mesh in the scene. It directly composites BVH.
    class Triangle
    {
    public:
        /// @brief Get the bounding box of the triangle.
        /// @return Bounding box of the triangle.
        const BoundingBox BBox() const;
        std::array<Vector3f, 3> vert;
        std::array<Vector3f, 3> norm;
        std::array<Vector2f, 3> uv;

        Triangle(const std::array<Vector3f, 3> &vert_, const std::array<Vector3f, 3> &norm_, const std::array<Vector2f, 3> &uv_, Mesh *const parent_);
        /// @brief Do ray-triangle intersection test in WORLD SPACE.
        /// @param ray Incoming ray.
        /// @param t Distance.
        /// @param u Barycentric U.
        /// @param v Barycentric V.
        /// @return Intersected(TRUE) or not(FALSE).
        const bool Intersect(const Ray &ray, float &t, float &u, float &v) const;
        /// @brief Get sample point in WORLD SPACE.
        /// @return
        const Vector3f GetSamplePoint() const;
        /// @brief Get cached area.
        /// @return
        const float AreaC() const;
        /// @brief (Deprecated) Get cached tangent.
        /// @return Returns geometrical tangent. Deprecated for not supporting gourand interpolation.
        [[deprecated]] const Vector3f TangentC() const;
        /// @brief Get cached normal.
        /// @return
        const Vector3f NormalC(const float u, const float v) const;
        /// @brief Get geometrical normal.
        /// @return
        const Vector3f GeometricalNormalC() const;

        /// @brief Update cache. Should be evaluated after O2W matrix having been changed.
        void UpdateCache();

        /// @brief [OpenGL Extension] Generate VBO of the mesh.
        /// @param attrib_list OpenGL attribute list.
        /// @return VBO.
        const std::vector<float> GetVBO(const std::vector<GLAttributeObject> &attrib_list) const;

        /// @brief [OpenGL Extension] Generate and append VBO of the mesh into a existing list.
        /// @param target Existing VBO.
        /// @param attrib_list OpenGL attribute list.
        /// @return No return.
        const void AppendVBO(std::vector<float> &target, const std::vector<GLAttributeObject> &attrib_list) const;

        Mesh *parent;

    private:
        /// @brief Calculate tangent.
        /// @return
        const Vector3f Tangent() const;
        /// @brief Calculate normal.
        /// @return
        const Vector3f Normal() const;
        /// @brief Calculate area.
        /// @return
        const float Area() const;

        Vector3f v0v1_w;
        Vector3f v0v2_w;
        std::array<Vector3f, 3> vert_w;

        float area;
        Vector3f normal;
        Vector3f tangent;
    };

    /// @brief Polygon class. Used by PCG Mesh & Importer. Provides a function to convert to triangles.
    struct Polygon
    {
        std::size_t V;
        std::vector<Vector3f> vert;
        std::vector<Vector3f> norm;
        std::vector<Vector2f> uv;

        Mesh *parent;

        /// @brief Construct a polygon object.
        /// @param V_ Count of vertices.
        /// @param vert_ List of vertices.
        /// @param norm_ List of normals.
        /// @param uv_ List of UVs.
        /// @param parent_ Parent object.
        Polygon(const std::size_t V_, const std::vector<Vector3f> &vert_, const std::vector<Vector3f> &norm_, const std::vector<Vector2f> &uv_, Mesh *parent_);

        /// @brief Convert polygon to triangles.
        /// @warning DO NOT DISCARD FOR POTENTIAL MEMORY LEAKAGE.
        /// @return List of triangles.
        [[nodiscard]] std::vector<Triangle *> ConvertToTriangle() const;
    };

    /// @brief Mesh class. Represents a set of triangles.
    class Mesh : public Geometry
    {
    public:
        std::vector<Triangle *> tris;
        PrincipledBSDF *tex;
        bool smooth{false};

        /// @brief Set object to world matrix.
        /// @param object_to_world O2W Matrix, should be an AFFINE matrix.
        virtual void SetO2W(const Matrix4x4f &object_to_world_) override;

        /// @brief [OpenGL Extension] Generate VBO of the mesh.
        /// @param attrib_list OpenGL attribute list.
        /// @return VBO.
        [[nodiscard]] const std::vector<float> GetVBO(const std::vector<GLAttributeObject> &attrib_list) const;

        /// @brief [OpenGL Extension] Generate and append VBO of the mesh into a existing list.
        /// @param target Existing VBO.
        /// @param attrib_list OpenGL attribute list.
        /// @return No return.
        const void AppendVBO(std::vector<float> &target, const std::vector<GLAttributeObject> &attrib_list) const;
    };

    /// @brief Interface for abstract lights.
    struct Light : public Geometry
    {
        Vector3f color = Vector3f::White;
        float intensity = 1.0f;

        Light(Vector3f color_ = Vector3f::White, float intensity_ = 1.0f);

        virtual const Vector3f GetSamplePoint() const = 0;
    };

    /// @brief Delta light.
    struct DeltaLight : public Light
    {
        /// @brief Create a delta light.
        /// @param color_ Base color of light.
        /// @param intensity_ Intensity of light.
        DeltaLight(Vector3f color_ = Vector3f::White, float intensity_ = 1.0f);

        /// @brief Sampling the light.
        /// @return
        virtual const Vector3f GetSamplePoint() const override final;
    };

    /// @brief Directional light. Pointing at (0,0,-1) in object space.
    struct DirectionalLight : public Light
    {
        /// @brief Create a directional light.
        /// @param color_ Base color of light.
        /// @param intensity_ Intensity of light.
        DirectionalLight(Vector3f color_ = Vector3f::White, float intensity_ = 1.0f);

        /// @brief Sampling the light.
        /// @return
        virtual const Vector3f GetSamplePoint() const override final;
    };
};

#endif // OBJECT_H