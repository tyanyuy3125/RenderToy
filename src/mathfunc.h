/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MATHFUNC_H
#define MATHFUNC_H

#define EPS 1e-5
#define PI 3.14159265358979323846L

#include <array>

namespace OpenPT
{
    class Vector3f
    {
    public:
        float x, y, z;

        Vector3f();
        Vector3f(const float x_, const float y_, const float z_);
        Vector3f(const std::array<float, 3> &triple);

        float Length() const;

        void Normalize();
        const Vector3f Normalized() const;

        float &operator[](const int i);

        const Vector3f operator+(const Vector3f &a) const;
        const Vector3f operator-(const Vector3f &a) const;
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

    class Vector2f
    {
    public:
        float x, y;

        Vector2f();
        Vector2f(const float x_, const float y_);
        Vector2f(const std::array<float, 2> &tuple);

        float Length() const;

        void Normalize();
        const Vector2f Normalized() const;

        float &operator[](const int i);

        const Vector2f operator+(const Vector2f &a) const;
        const Vector2f operator-(const Vector2f &a) const;
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

    class Vector4f
    {
    public:
        float x, y, z, w;

        Vector4f();
        Vector4f(const float x_, const float y_, const float z_, const float w_);
        Vector4f(const std::array<float, 4> &quadruple);

        float Length() const;

        void Normalize();
        const Vector4f Normalized() const;

        float &operator[](const int i);

        const Vector4f operator+(const Vector4f &a) const;
        const Vector4f operator-(const Vector4f &a) const;
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

};

#endif