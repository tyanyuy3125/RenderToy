/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Last edited on 2023/1/16
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MATHFUNC_H
#define MATHFUNC_H

#define EPS 1e-5
#define PI 3.14159265358979323846L

#include <array>
#include <istream>

namespace OpenPT
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

        float &operator[](const int i);
        const float operator[](const int i) const;

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

        float x, y, z;

        Vector3f();
        Vector3f(const float x_, const float y_, const float z_);
        Vector3f(const std::array<float, 3> &triple);
        Vector3f(const Vector2f &vec2, float val3);

        float Length() const;

        void Normalize();
        const Vector3f Normalized() const;

        float &operator[](const int i);
        const float operator[](const int i) const;

        const Vector3f operator+(const Vector3f &a) const;
        const Vector3f operator-(const Vector3f &a) const;
        const Vector3f operator-(void) const;
        const Vector3f operator*(const float a) const;
        const Vector3f operator/(const float a) const;

        const Vector3f &operator+=(const Vector3f &a);
        const Vector3f &operator-=(const Vector3f &a);
        const Vector3f &operator*=(const float a);
        const Vector3f &operator/=(const float a);

        const bool operator==(const Vector3f &a) const;

        const static float Dot(const Vector3f &a, const Vector3f &b);
        const static Vector3f Cross(const Vector3f &a, const Vector3f &b);
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
        Vector4f(const Vector3f &vec3, float val4);

        float Length() const;

        void Normalize();
        const Vector4f Normalized() const;

        float &operator[](const int i);
        const float operator[](const int i) const;

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

        Vector3f &operator[](const int i);
        const Vector3f &operator[](const int i) const;

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

        Vector4f &operator[](const int i);
        const Vector4f &operator[](const int i) const;

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

        Vector2f &operator[](const int i);
        const Vector2f &operator[](const int i) const;

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
    const float InchToMM(const float inch);
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
}
#endif // MATHFUNC_H