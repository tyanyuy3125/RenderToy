/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#ifndef MATHFUNC_H
#define MATHFUNC_H

#include <array>

namespace OpenPT
{
    class Vector3f
    {
    private:
        float value[3];

    public:
        Vector3f();
        Vector3f(const float x, const float y, const float z);
        Vector3f(const std::array<float,3> &triple);

        float &x();
        float &y();
        float &z();

        // float Length();

        // void Normalize();
        // const Vector3f Normalized();

        // float & operator[](const int i);

        // const Vector3f operator+(const float &a);
        // const Vector3f operator-(const float &a);
        // const Vector3f operator*(const float &a);
        // const Vector3f operator/(const float &a);

        // const Vector3f &operator+=(const Vector3f &a);
        // const Vector3f &operator-=(const Vector3f &a);
        // const Vector3f &operator*=(const float a);
        // const Vector3f &operator/=(const float a);

        // const bool operator==(const float &a);
        // const bool operator=(const float &a);
    };
};

#endif