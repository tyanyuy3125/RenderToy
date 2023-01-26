/*
 *  RenderToy - Math Function Module
 *  File created on 2023/1/10
 *  Last edited on 2023/1/16
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MATHFUNC_H
#define MATHFUNC_H

#define EPS 1e-7
#define INF 1e7

#include <array>
#include <istream>
#include <stack>
#include <cmath>
#include <algorithm>

#include "random.h"

namespace RenderToy
{
    struct Size;
    struct Point;

#pragma region Basic Vector Math
    class Vector2f
    {
    public:
        const static Vector2f O;
        const static Vector2f X;
        const static Vector2f Y;

        float x, y;

        Vector2f();
        Vector2f(const float x_, const float y_);
        Vector2f(const std::array<float, 2> &tuple);
        Vector2f(const Point &p);

        float Length() const;

        void Normalize();
        const Vector2f Normalized() const;

        float &operator[](const size_t i);
        const float operator[](const size_t i) const;

        const Vector2f operator+(const Vector2f &a) const;
        const Vector2f operator-(const Vector2f &a) const;
        const Vector2f operator-(void) const;
        const Vector2f operator*(const float a) const;
        const Vector2f operator/(const float a) const;

        const Vector2f &operator+=(const Vector2f &a);
        const Vector2f &operator-=(const Vector2f &a);
        const Vector2f &operator*=(const float a);
        const Vector2f &operator/=(const float a);

        const bool operator==(const Vector2f &a) const;

        const static float Dot(const Vector2f &a, const Vector2f &b);
        const static float Cross(const Vector2f &a, const Vector2f &b);
    };

    const Vector2f operator*(const float lambda, const Vector2f &a);
    std::istream &operator>>(std::istream &is, Vector2f &a);

    class Vector3f
    {
    public:
        const static Vector3f O;
        const static Vector3f X;
        const static Vector3f Y;
        const static Vector3f Z;
        const static Vector3f White;
        const static Vector3f Inf;

        float x, y, z;

        Vector3f();
        Vector3f(const float x_, const float y_, const float z_);
        Vector3f(const std::array<float, 3> &triple);
        Vector3f(const Vector2f &vec2, float val3);
        Vector3f(const float val);

        float Length() const;

        void Normalize();
        const Vector3f Normalized() const;

        float &operator[](const size_t i);
        const float operator[](const size_t i) const;

        const Vector3f operator+(const Vector3f &a) const;
        const Vector3f operator-(const Vector3f &a) const;
        const Vector3f operator-(void) const;
        const Vector3f operator*(const float a) const;
        const Vector3f operator*(const Vector3f &vec) const;
        const Vector3f operator/(const float a) const;

        const Vector3f &operator+=(const Vector3f &a);
        const Vector3f &operator-=(const Vector3f &a);
        const Vector3f &operator*=(const float a);
        const Vector3f &operator/=(const float a);

        const bool operator==(const Vector3f &a) const;
        const bool operator!=(const Vector3f &a) const;

        const static float Dot(const Vector3f &a, const Vector3f &b);
        const static Vector3f Cross(const Vector3f &a, const Vector3f &b);
        /// @brief GLSL-like respective pow.
        /// @param a
        /// @param b
        /// @return
        const static Vector3f Pow(const Vector3f &a, const Vector3f &b);
        const static Vector3f Mix(const Vector3f &x, const Vector3f &y, const Vector3f &a);

        const float Dot(const Vector3f &a) const;
        const Vector3f Cross(const Vector3f &a) const;
    };

    const Vector3f operator*(const float lambda, const Vector3f &a);
    std::istream &operator>>(std::istream &is, Vector3f &a);

    class Vector4f
    {
    public:
        const static Vector4f O;
        const static Vector4f X;
        const static Vector4f Y;
        const static Vector4f Z;
        const static Vector4f W;

        float x, y, z, w;

        Vector4f();
        Vector4f(const float x_, const float y_, const float z_, const float w_);
        Vector4f(const std::array<float, 4> &quadruple);
        Vector4f(const Vector3f &Vector3f, float val4);

        float Length() const;

        void Normalize();
        const Vector4f Normalized() const;

        float &operator[](const size_t i);
        const float operator[](const size_t i) const;

        const Vector4f operator+(const Vector4f &a) const;
        const Vector4f operator-(const Vector4f &a) const;
        const Vector4f operator-(void) const;
        const Vector4f operator*(const float a) const;
        const Vector4f operator/(const float a) const;

        const Vector4f &operator+=(const Vector4f &a);
        const Vector4f &operator-=(const Vector4f &a);
        const Vector4f &operator*=(const float a);
        const Vector4f &operator/=(const float a);

        const bool operator==(const Vector4f &a) const;

        const static float Dot(const Vector4f &a, const Vector4f &b);
        const static Vector4f Cross(const Vector4f &a, const Vector4f &b);
    };

    const Vector4f operator*(const float lambda, const Vector4f &a);

#pragma endregion

#pragma region Basic Matrix Math
    class Matrix3x3f
    {
    private:
        Vector3f row[3];

    public:
        const static Matrix3x3f I;
        const static Matrix3x3f O;

        Matrix3x3f();
        Matrix3x3f(const Matrix3x3f &a);
        Matrix3x3f(Vector3f r0, Vector3f r1, Vector3f r2);
        Matrix3x3f(std::array<Vector3f, 3> triple);

        Vector3f &operator[](const size_t i);
        const Vector3f &operator[](const size_t i) const;

        const bool operator==(const Matrix3x3f &a) const;

        const Matrix3x3f operator+(const Matrix3x3f &a) const;
        const Matrix3x3f operator-(const Matrix3x3f &a) const;
        const Matrix3x3f operator-(void) const;
        const Vector3f operator*(const Vector3f &a) const;
        const Matrix3x3f operator*(const Matrix3x3f &a) const;

        const Matrix3x3f &operator+=(const Matrix3x3f &a);
        const Matrix3x3f &operator-=(const Matrix3x3f &a);

        void Transpose();
        const Matrix3x3f Transposed() const;

        const static float Determinant(const Matrix3x3f &mat);
    };

    class Matrix4x4f
    {
    private:
        Vector4f row[4];

    public:
        const static Matrix4x4f I;
        const static Matrix4x4f O;

        Matrix4x4f();
        Matrix4x4f(const Matrix4x4f &a);
        Matrix4x4f(Vector4f r0, Vector4f r1, Vector4f r2, Vector4f r3);
        Matrix4x4f(std::array<Vector4f, 4> quadruple);

        Vector4f &operator[](const size_t i);
        const Vector4f &operator[](const size_t i) const;

        const bool operator==(const Matrix4x4f &a) const;

        const Matrix4x4f operator+(const Matrix4x4f &a) const;
        const Matrix4x4f operator-(const Matrix4x4f &a) const;
        const Matrix4x4f operator-(void) const;
        const Vector4f operator*(const Vector4f &a) const;
        const Matrix4x4f operator*(const Matrix4x4f &a) const;

        const Matrix4x4f &operator+=(const Matrix4x4f &a);
        const Matrix4x4f &operator-=(const Matrix4x4f &a);

        void Transpose();
        const Matrix4x4f Transposed() const;
        const Matrix3x3f ComplementMinor(const int i, const int j) const;
    };

    class Matrix2x2f
    {
    private:
        Vector2f row[2];

    public:
        const static Matrix2x2f I;
        const static Matrix2x2f O;

        Matrix2x2f();
        Matrix2x2f(const Matrix2x2f &a);
        Matrix2x2f(Vector2f r0, Vector2f r1);
        Matrix2x2f(std::array<Vector2f, 2> tuple);

        Vector2f &operator[](const size_t i);
        const Vector2f &operator[](const size_t i) const;

        const bool operator==(const Matrix2x2f &a) const;

        const Matrix2x2f operator+(const Matrix2x2f &a) const;
        const Matrix2x2f operator-(const Matrix2x2f &a) const;
        const Matrix2x2f operator-(void) const;
        const Vector2f operator*(const Vector2f &a) const;
        const Matrix2x2f operator*(const Matrix2x2f &a) const;

        const Matrix2x2f &operator+=(const Matrix2x2f &a);
        const Matrix2x2f &operator-=(const Matrix2x2f &a);

        void Transpose();
        const Matrix2x2f Transposed() const;

        const static float Determinant(const Matrix2x2f &mat);
    };
#pragma endregion

#pragma region Unit Conversion
    struct Convert
    {
        Convert() = delete;
        Convert(const Convert &) = delete;
        Convert(const Convert &&) = delete;

        static const float InchToMM(const float inch);
        static const float DegreeToRadians(const float deg);
        static const Vector3f BlackBody(const float t);
        static const Vector3f XYZToSRGB(const Vector3f &x);

        static const float RGBToLuminance(const Vector3f &vec);
        static const Vector3f Tonemap(const Vector3f &vec, const float limit);
    };
#pragma endregion

#pragma region Drawing Math
    struct Point
    {
        int x, y;

        Point();
        Point(int x_, int y_);
        Point(const Vector2f &vec);
        Point(const std::array<int, 2> &tuple);
    };

    struct Size
    {
        int width, height;

        Size();
        Size(int width_, int height_);
        Size(const Vector2f &vec);
        Size(const std::array<int, 2> &tuple);

        const int Area() const;
        const float AspectRatio() const;
    };

#pragma endregion

    class Matrix4x4fStack
    {
    private:
        std::stack<Matrix4x4f> accumulated;

    public:
        Matrix4x4fStack();

        void Push(const Matrix4x4f &mat4);
        void Pop();

        void Transform(Vector3f &Vector3f) const;
    };

    class AffineTransformation
    {
    public:
        AffineTransformation() = delete;
        AffineTransformation(const AffineTransformation &) = delete;
        AffineTransformation(const AffineTransformation &&) = delete;

        const static Matrix4x4f Translation(const Vector3f &origin);

        /// @brief Use Blender XYZ-Euler convention, matrix representation of intrinsic transformations: Z*Y*X*v.
        /// @param euler_xyz XYZ-Euler angles, in radius.
        /// @return
        const static Matrix4x4f RotationEulerXYZ(const Vector3f &euler_xyz);
    };

    const float SSE_InvSqrt(const float number);

    float GTR1(float NDotH, float a);

    Vector3f SampleGTR1(float rgh, float r1, float r2);

    float GTR2(float NDotH, float a);

    Vector3f SampleGTR2(float rgh, float r1, float r2);

    Vector3f SampleGGXVNDF(Vector3f V, float rgh, float r1, float r2);

    float GTR2Aniso(float NDotH, float HDotX, float HDotY, float ax, float ay);

    Vector3f SampleGTR2Aniso(float ax, float ay, float r1, float r2);

    float SmithG(float NDotV, float alphaG);

    float SmithGAniso(float NDotV, float VDotX, float VDotY, float ax, float ay);

    float SchlickFresnel(float u);

    float DielectricFresnel(float cosThetaI, float eta);

    Vector3f CosineSampleHemisphere(float r1, float r2);

    Vector3f UniformSampleHemisphere(float r1, float r2);

    Vector3f UniformSampleSphere(float r1, float r2);

    float PowerHeuristic(float a, float b);

    void Onb(const Vector3f N, Vector3f &T, Vector3f &B);

    Vector3f ToWorld(Vector3f X, Vector3f Y, Vector3f Z, Vector3f V);

    Vector3f ToLocal(Vector3f X, Vector3f Y, Vector3f Z, Vector3f V);

    // template <typename T>
    // const T Mix(const T x, const T y, const T a);

    template <typename T>
    const T Mix(const T x, const T y, const T a)
    {
        return x * (T(1) - a) + y * a;
    }

    float Luminance(Vector3f c);

    const Vector3f Reflect(const Vector3f &incidentVec, const Vector3f &normal);

    const Vector3f Refract(const Vector3f &incidentVec, const Vector3f &normal, float eta);

    struct RayState
    {
        bool isRefracted;
        bool hasBeenRefracted;
        float lastIOR;
        RayState();
    };
}
#endif // MATHFUNC_H