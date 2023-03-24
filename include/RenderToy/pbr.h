#ifndef PBR_H
#define PBR_H

#include "rtmath.h"

namespace RenderToy
{

#pragma region PBR
    const float GTR1(const float N_dot_H, const float a);
    const Vector3f SampleGTR1(const float rgh, const float r1, const float r2);
    const float GTR2(const float N_dot_H, const float a);
    const Vector3f SampleGTR2(const float rgh, const float r1, const float r2);
    const Vector3f SampleGGXVNDF(const Vector3f V, const float rgh, const float r1, const float r2);
    const float GTR2Aniso(const float N_dot_H, const float H_dot_X, const float H_dot_Y, const float ax, const float ay);
    const Vector3f SampleGTR2Aniso(const float ax, const float ay, const float r1, const float r2);
    const float SmithG(const float N_dot_V, const float alpha_g);
    const float SmithGAniso(const float N_dot_V, const float V_dot_X, const float V_dot_Y, const float ax, const float ay);
    const float SchlickFresnel(const float u);
    const float DielectricFresnel(const float cos_theta_i, const float eta);

    const Vector3f CosineSampleHemisphere(const float r1, const float r2);
    const Vector3f UniformSampleHemisphere(const float r1, const float r2);
    const Vector3f UniformSampleSphere(const float r1, const float r2);

    const float PowerHeuristic(const float a, const float b);
    void Onb(const Vector3f N, Vector3f &T, Vector3f &B);

    const Vector3f ToWorld(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V);
    const Vector3f ToLocal(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V);
    const Vector3f Refract(const Vector3f &incident_vec, const Vector3f &normal, float eta);
    const Vector3f Reflect(const Vector3f &incident_vec, const Vector3f &normal);

#pragma endregion PBR

}
#endif // PBR_H