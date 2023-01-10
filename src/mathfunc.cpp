/*
 *  OpenPT - Math Function Module
 *  File created on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#include <cmath>

#include "mathfunc.h"

using namespace OpenPT;

Vector3f::Vector3f(const float x, const float y, const float z)
{
    value[0] = x;
    value[1] = y;
    value[2] = z;
}

Vector3f::Vector3f() : Vector3f(0.0f, 0.0f, 0.0f) {}

Vector3f::Vector3f(const std::array<float, 3> &triple) : Vector3f(triple[0], triple[1], triple[2]) {}

float &Vector3f::x()
{
    return value[0];
}

float &Vector3f::y()
{
    return value[1];
}

float &Vector3f::z()
{
    return value[2];
}

// float Vector3f::Length()
// {
//     return sqrt(value[0] * value[0] +
//                 value[1] * value[1] +
//                 value[2] * value[2]);
// }

// void Vector3f::Normalize(){
//     (*this) /= Length();
// }

// const Vector3f Normalized(){
    
// }

// const Vector3f &Vector3f::operator*=(const float a){
//     value[0] *= a;
//     value[1] *= a;
//     value[2] *= a;
//     return (*this);
// }

// const Vector3f &Vector3f::operator/=(const float a){
//     value[0] /= a;
//     value[1] /= a;
//     value[2] /= a;
//     return (*this);
// }