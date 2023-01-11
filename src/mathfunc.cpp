/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#include <cmath>

#include "mathfunc.h"

namespace OpenPT
{
    Vector3f::Vector3f(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}
    Vector3f::Vector3f() : Vector3f(0.0f, 0.0f, 0.0f) {}
    Vector3f::Vector3f(const std::array<float, 3> &triple) : Vector3f(triple[0], triple[1], triple[2]) {}

    float Vector3f::Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    void Vector3f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector3f Vector3f::Normalized() const
    {
        return Vector3f(*this) / Length();
    }

    float &Vector3f::operator[](const int i)
    {
        return *((&x) + i);
    }

    const Vector3f Vector3f::operator+(const Vector3f &a) const
    {
        return Vector3f(x + a.x, y + a.y, z + a.z);
    }

    const Vector3f Vector3f::operator-(const Vector3f &a) const
    {
        return Vector3f(x - a.x, y - a.y, z - a.z);
    }

    const Vector3f Vector3f::operator*(const float a) const
    {
        return Vector3f(x * a, y * a, z * a);
    }

    const Vector3f Vector3f::operator/(const float a) const
    {
        return Vector3f(x / a, y / a, z / a);
    }

    const Vector3f &Vector3f::operator+=(const Vector3f &a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        return (*this);
    }

    const Vector3f &Vector3f::operator-=(const Vector3f &a)
    {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        return (*this);
    }

    const Vector3f &Vector3f::operator*=(const float a)
    {
        x *= a;
        y *= a;
        z *= a;
        return (*this);
    }

    const Vector3f &Vector3f::operator/=(const float a)
    {
        x /= a;
        y /= a;
        z /= a;
        return (*this);
    }

    const bool Vector3f::operator==(const Vector3f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS) && (std::abs(z - a.z) < EPS));
    }

    const Vector3f operator*(const float lambda, const Vector3f &a)
    {
        return a * lambda;
    }

    const float Vector3f::Dot(const Vector3f &a, const Vector3f &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    const Vector3f Vector3f::Cross(const Vector3f &a, const Vector3f &b)
    {
        return Vector3f(a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);
    }

    std::istream &operator>>(std::istream &is, Vector3f &a)
    {
        is >> a.x >> a.y >> a.z;
        return is;
    }

    Vector2f::Vector2f(const float x_, const float y_) : x(x_), y(y_) {}
    Vector2f::Vector2f() : Vector2f(0.0f, 0.0f) {}
    Vector2f::Vector2f(const std::array<float, 2> &tuple) : Vector2f(tuple[0], tuple[1]) {}

    float Vector2f::Length() const
    {
        return sqrt(x * x + y * y);
    }

    void Vector2f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector2f Vector2f::Normalized() const
    {
        return Vector2f(*this) / Length();
    }

    float &Vector2f::operator[](const int i)
    {
        return *((&x) + i);
    }

    const Vector2f Vector2f::operator+(const Vector2f &a) const
    {
        return Vector2f(x + a.x, y + a.y);
    }

    const Vector2f Vector2f::operator-(const Vector2f &a) const
    {
        return Vector2f(x - a.x, y - a.y);
    }

    const Vector2f Vector2f::operator*(const float a) const
    {
        return Vector2f(x * a, y * a);
    }

    const Vector2f Vector2f::operator/(const float a) const
    {
        return Vector2f(x / a, y / a);
    }

    const Vector2f &Vector2f::operator+=(const Vector2f &a)
    {
        x += a.x;
        y += a.y;
        return (*this);
    }

    const Vector2f &Vector2f::operator-=(const Vector2f &a)
    {
        x -= a.x;
        y -= a.y;
        return (*this);
    }

    const Vector2f &Vector2f::operator*=(const float a)
    {
        x *= a;
        y *= a;
        return (*this);
    }

    const Vector2f &Vector2f::operator/=(const float a)
    {
        x /= a;
        y /= a;
        return (*this);
    }

    const bool Vector2f::operator==(const Vector2f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS));
    }

    const Vector2f operator*(const float lambda, const Vector2f &a)
    {
        return a * lambda;
    }

    const float Vector2f::Dot(const Vector2f &a, const Vector2f &b)
    {
        return a.x * b.x + a.y * b.y;
    }

    const float Vector2f::Cross(const Vector2f &a, const Vector2f &b)
    {
        return a.x * b.y - a.y * b.x;
    }

    std::istream &operator>>(std::istream &is, Vector2f &a)
    {
        is >> a.x >> a.y;
        return is;
    }

    Vector4f::Vector4f(const float x_, const float y_, const float z_, const float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Vector4f::Vector4f() : Vector4f(0.0f, 0.0f, 0.0f, 0.0f) {}
    Vector4f::Vector4f(const std::array<float, 4> &quadruple) : Vector4f(quadruple[0], quadruple[1], quadruple[2], quadruple[3]) {}

    float Vector4f::Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    void Vector4f::Normalize()
    {
        (*this) /= Length();
    }

    const Vector4f Vector4f::Normalized() const
    {
        return Vector4f(*this) / Length();
    }

    float &Vector4f::operator[](const int i)
    {
        return *((&x) + i);
    }

    const Vector4f Vector4f::operator+(const Vector4f &a) const
    {
        return Vector4f(x + a.x, y + a.y, z + a.z, w + a.w);
    }

    const Vector4f Vector4f::operator-(const Vector4f &a) const
    {
        return Vector4f(x - a.x, y - a.y, z - a.z, w - a.w);
    }

    const Vector4f Vector4f::operator*(const float a) const
    {
        return Vector4f(x * a, y * a, z * a, w * a);
    }

    const Vector4f Vector4f::operator/(const float a) const
    {
        return Vector4f(x / a, y / a, z / a, w / a);
    }

    const Vector4f &Vector4f::operator+=(const Vector4f &a)
    {
        x += a.x;
        y += a.y;
        z += a.z;
        w += a.w;
        return (*this);
    }

    const Vector4f &Vector4f::operator-=(const Vector4f &a)
    {
        x -= a.x;
        y -= a.y;
        z -= a.z;
        w -= a.w;
        return (*this);
    }

    const Vector4f &Vector4f::operator*=(const float a)
    {
        x *= a;
        y *= a;
        z *= a;
        w *= a;
        return (*this);
    }

    const Vector4f &Vector4f::operator/=(const float a)
    {
        x /= a;
        y /= a;
        z /= a;
        w /= a;
        return (*this);
    }

    const bool Vector4f::operator==(const Vector4f &a) const
    {
        return ((std::abs(x - a.x) < EPS) && (std::abs(y - a.y) < EPS) && (std::abs(z - a.z) < EPS) && (std::abs(w - a.w) < EPS));
    }

    const Vector4f operator*(const float lambda, const Vector4f &a)
    {
        return a * lambda;
    }

    const float Vector4f::Dot(const Vector4f &a, const Vector4f &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    const Vector4f Vector4f::Cross(const Vector4f &a, const Vector4f &b)
    {
        // TODO <tianyu@illumiart.net>: Implement 4D-vector cross product using matrix operation.
        return Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
    }

    Vector4f &Matrix4x4f::operator[](const int i)
    {
        return row[i];
    }
}