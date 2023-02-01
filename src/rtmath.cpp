#include <cmath>
#include <immintrin.h>

#include "rtmath.h"

/// @brief Multiply 4 floats by another 4 floats.
/// @tparam offset Offset from the first float.
/// @param p1 Memory address of first float in the first 4 floats.
/// @param p2 Memory address of first float in the second 4 floats.
/// @return
template <int offset>
inline __m128 mul4(const float *p1, const float *p2)
{
    constexpr int lanes = offset * 8;
    const __m128 a = _mm_loadu_ps(p1 + lanes);
    const __m128 b = _mm_loadu_ps(p2 + lanes);
    return _mm_mul_ps(a, b);
}

namespace RenderToy
{

    const float SSE_InvSqrt(const float number)
    {
        __m128 _srcReisger = _mm_set1_ps(number);
        __m128 _dstRegister = _mm_rsqrt_ps(_srcReisger);
        float array[4]; // 128-bit xmm register (4x32-bit float)
        _mm_storeu_ps(array, _dstRegister);
        return array[0]; // The result at all the elements are the same
    }

    const float Luma(const Vector3f &color)
    {
        return Vector3f::Dot(color, Vector3f(0.299f, 0.587f, 0.114f));
    }

    const float Convert::InchToMM(const float inch)
    {
        return inch * 25.4f;
    }

    const float Convert::DegreeToRadians(const float deg)
    {
        return deg * (M_PIf32 / 180.0f);
    }

    const Vector3f Convert::BlackBody(const float t)
    {
        // MIT License
        // Copyright (c) 2020 Christopher J. Howard
        // https://www.shadertoy.com/view/tsKczy

        // Approximate the Planckian locus in CIE 1960 UCS color space (Krystek's algorithm)
        float tt = t * t;
        float u = (0.860117757 + 1.54118254e-4 * t + 1.28641212e-7 * tt) / (1.0 + 8.42420235e-4 * t + 7.08145163e-7 * tt);
        float v = (0.317398726 + 4.22806245e-5 * t + 4.20481691e-8 * tt) / (1.0 - 2.89741816e-5 * t + 1.61456053e-7 * tt);

        // CIE 1960 UCS -> CIE xyY, Y = 1
        Vector2f xyy = Vector2f(3.0f * u, 2.0f * v) / (2.0 * u - 8.0 * v + 4.0);

        // CIE xyY -> CIE XYZ
        Vector3f xyz = Vector3f(xyy.x() / xyy.y(), 1.0f, (1.0f - xyy.x() - xyy.y()) / xyy.y());

        // CIE XYZ -> linear sRGB
        Vector3f srgb = XYZToSRGB(xyz);

        // Normalize RGB to preserve chromaticity
        return srgb / std::max(srgb.x(), std::max(srgb.y(), srgb.z()));
    }

    const Vector3f Convert::XYZToSRGB(const Vector3f &x)
    {
        // Source: http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
        const Matrix3x3f matrix = {
            {3.2404542f, -1.5371385f, -0.4985314f},
            {-0.9692660f, 1.8760108f, 0.0415560f},
            {0.0556434f, -0.2040259f, 1.0572252f}};
        return matrix * x;
    }

    const float Convert::RGBToLuminance(const Vector3f &vec)
    {
        return 0.212671 * vec.x() + 0.715160 * vec.y() + 0.072169 * vec.z();
    }

    const Vector3f Convert::Tonemap(const Vector3f &vec, const float limit)
    {
        return vec * 1.0f / (1.0f + RGBToLuminance(vec) / limit);
    }

    Point::Point()
        : x(0), y(0)
    {
    }

    Point::Point(int x_, int y_)
        : x(x_), y(y_)
    {
    }

    Point::Point(const Vector2f &vec)
        : x(int(vec.x())), y(int(vec.y()))
    {
    }

    Point::Point(const std::array<int, 2> &tuple)
        : x(tuple[0]), y(tuple[1])
    {
    }

    Size::Size()
        : width(0), height(0)
    {
    }

    Size::Size(int x_, int y_)
        : width(x_), height(y_)
    {
    }

    Size::Size(const Vector2f &vec)
        : width(int(vec.x())), height(int(vec.y()))
    {
    }

    Size::Size(const std::array<int, 2> &tuple)
        : width(tuple[0]), height(tuple[1])
    {
    }

    const int Size::Area() const
    {
        return width * height;
    }

    const float Size::AspectRatio() const
    {
        return float(width) / float(height);
    }

    void Matrix4x4fStack::Push(const Matrix4x4f &mat4)
    {
        accumulated.push(mat4 * accumulated.top());
    }

    Matrix4x4fStack::Matrix4x4fStack()
    {
        accumulated.push(Matrix4x4f::I);
    }

    void Matrix4x4fStack::Pop()
    {
        if (accumulated.size() == 1)
        {
            return;
        }
        accumulated.pop();
    }

    void Matrix4x4fStack::Transform(Vector3f &vec3) const
    {
        // Vector4f vec4(vec3, 1.0f);
        Vector4f vec4(vec3.x(), vec3.y(), vec3.z(), 1.0f); // TODO: Optimize.
        vec4 = accumulated.top() * vec4;
        vec3 = Vector3f(vec4[0], vec4[1], vec4[2]);
    }

    const Matrix4x4f AffineTransformation::Translation(const Vector3f &origin)
    {
        Matrix4x4f ret(Matrix4x4f::I);
        for (int i = 0; i < 3; ++i)
        {
            ret[i][3] = origin[i];
        }
        return ret;
    }

    const Matrix4x4f AffineTransformation::RotationEulerXYZ(const Vector3f &euler_xyz)
    {
        // TODO: Optimize.

        auto x = euler_xyz.x();
        auto y = euler_xyz.y();
        auto z = euler_xyz.z();
        Matrix4x4f Z = {{std::cos(z), -std::sin(z), 0.0f, 0.0f},
                        {std::sin(z), std::cos(z), 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        Matrix4x4f Y = {{std::cos(y), 0.0f, -std::sin(y), 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {std::sin(y), 0.0f, std::cos(y), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        Matrix4x4f X = {{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, std::cos(x), -std::sin(x), 0.0f},
                        {0.0f, std::sin(x), std::cos(x), 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};

        return Z * Y * X;
    }

    const float GTR1(const float NDotH, const float a)
    {
        if (a >= 1.0f)
            return (1.0f / M_PIf32);
        float a2 = a * a;
        float t = 1.0f + (a2 - 1.0f) * NDotH * NDotH;
        return (a2 - 1.0f) / (M_PIf32 * std::log(a2) * t);
    }

    const Vector3f SampleGTR1(const float rgh, const float r1, const float r2)
    {
        float a = std::max(0.001f, rgh);
        float a2 = a * a;

        float phi = r1 * 2.0f * M_PIf32;

        float cosTheta = std::sqrt((1.0f - std::pow(a2, 1.0f - r1)) / (1.0f - a2));
        float sinTheta = std::clamp(std::sqrt(1.0f - (cosTheta * cosTheta)), 0.0f, 1.0f);
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        return Vector3f(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
    }

    const float GTR2(const float NDotH, const float a)
    {
        float a2 = a * a;
        float t = 1.0f + (a2 - 1.0f) * NDotH * NDotH;
        return a2 / (M_PIf32 * t * t);
    }

    const Vector3f SampleGTR2(const float rgh, const float r1, const float r2)
    {
        float a = std::max(0.001f, rgh);

        float phi = r1 * 2.0f * M_PIf32;

        float cosTheta = std::sqrt((1.0f - r2) / (1.0f + (a * a - 1.0f) * r2));
        float sinTheta = std::clamp(std::sqrt(1.0f - (cosTheta * cosTheta)), 0.0f, 1.0f);
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        return Vector3f(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
    }

    const Vector3f SampleGGXVNDF(const Vector3f V, const float rgh, const float r1, const float r2)
    {
        Vector3f Vh = Vector3f(rgh * V.x(), rgh * V.y(), V.z()).Normalized();

        float lensq = Vh.x() * Vh.x() + Vh.y() * Vh.y();
        Vector3f T1 = lensq > 0.0f ? Vector3f(-Vh.y(), Vh.x(), 0.0f) * (1.0f / std::sqrt(lensq)) : Vector3f(1.0f, 0.0f, 0.0f);
        Vector3f T2 = Vector3f::Cross(Vh, T1);

        float r = sqrt(r1);
        float phi = 2.0f * M_PIf32 * r2;
        float t1 = r * std::cos(phi);
        float t2 = r * std::sin(phi);
        float s = 0.5f * (1.0f + Vh.z());
        t2 = (1.0f - s) * std::sqrt(1.0f - t1 * t1) + s * t2;

        Vector3f Nh = t1 * T1 + t2 * T2 + std::sqrt(std::max(0.0f, 1.0f - t1 * t1 - t2 * t2)) * Vh;

        return Vector3f(rgh * Nh.x(), rgh * Nh.y(), std::max(0.0f, Nh.z())).Normalized();
    }

    const float GTR2Aniso(const float NDotH, const float HDotX, const float HDotY, const float ax, const float ay)
    {
        float a = HDotX / ax;
        float b = HDotY / ay;
        float c = a * a + b * b + NDotH * NDotH;
        return 1.0f / (M_PIf32 * ax * ay * c * c);
    }

    const Vector3f SampleGTR2Aniso(const float ax, const float ay, const float r1, const float r2)
    {
        float phi = r1 * 2.0f * M_PIf32;

        float sinPhi = ay * std::sin(phi);
        float cosPhi = ax * std::cos(phi);
        float tanTheta = std::sqrt(r2 / (1.0f - r2));

        return Vector3f(tanTheta * cosPhi, tanTheta * sinPhi, 1.0f);
    }

    const float SmithG(const float NDotV, const float alphaG)
    {
        float a = alphaG * alphaG;
        float b = NDotV * NDotV;
        return (2.0f * NDotV) / (NDotV + std::sqrt(a + b - a * b));
    }

    const float SmithGAniso(const float NDotV, const float VDotX, const float VDotY, const float ax, const float ay)
    {
        float a = VDotX * ax;
        float b = VDotY * ay;
        float c = NDotV;
        return 1.0f / (NDotV + std::sqrt(a * a + b * b + c * c));
    }

    const float SchlickFresnel(const float u)
    {
        float m = std::clamp(1.0f - u, 0.0f, 1.0f);
        float m2 = m * m;
        return m2 * m2 * m;
    }

    const float DielectricFresnel(const float cosThetaI, const float eta)
    {
        float sinThetaTSq = eta * eta * (1.0f - cosThetaI * cosThetaI);

        // Total internal reflection
        if (sinThetaTSq > 1.0f)
            return 1.0f;

        float cosThetaT = std::sqrt(std::max(1.0f - sinThetaTSq, 0.0f));

        float rs = (eta * cosThetaT - cosThetaI) / (eta * cosThetaT + cosThetaI);
        float rp = (eta * cosThetaI - cosThetaT) / (eta * cosThetaI + cosThetaT);

        return 0.5f * (rs * rs + rp * rp);
    }

    const Vector3f CosineSampleHemisphere(const float r1, const float r2)
    {
        Vector3f dir;
        float r = std::sqrt(r1);
        float phi = 2.0f * M_PIf32 * r2;
        dir.x() = r * std::cos(phi);
        dir.y() = r * std::sin(phi);
        dir.z() = std::sqrt(std::max(0.0f, 1.0f - dir.x() * dir.x() - dir.y() * dir.y()));
        return dir;
    }

    const Vector3f UniformSampleHemisphere(const float r1, const float r2)
    {
        float r = std::sqrt(std::max(0.0f, 1.0f - r1 * r1));
        float phi = 2.0f * M_PIf32 * r2;
        return Vector3f(r * std::cos(phi), r * std::sin(phi), r1);
    }

    const Vector3f UniformSampleSphere(const float r1, const float r2)
    {
        float z = 1.0f - 2.0f * r1;
        float r = std::sqrt(std::max(0.0f, 1.0f - z * z));
        float phi = 2.0f * M_PIf32 * r2;
        return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
    }

    const float PowerHeuristic(const float a, const float b)
    {
        float t = a * a;
        return t / (b * b + t);
    }

    void Onb(const Vector3f N, Vector3f &T, Vector3f &B)
    {
        Vector3f up = std::abs(N.z()) < 0.999f ? Vector3f(0.0f, 0.0f, 1.0f) : Vector3f(1.0f, 0.0f, 0.0f);
        T = Vector3f::Cross(up, N).Normalized();
        B = Vector3f::Cross(N, T);
    }

    const Vector3f ToWorld(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V)
    {
        return V.x() * X + V.y() * Y + V.z() * Z;
    }

    const Vector3f ToLocal(const Vector3f X, const Vector3f Y, const Vector3f Z, const Vector3f V)
    {
        return Vector3f(Vector3f::Dot(V, X), Vector3f::Dot(V, Y), Vector3f::Dot(V, Z));
    }

    const Vector3f Reflect(const Vector3f &incidentVec, const Vector3f &normal)
    {
        return incidentVec - 2.0f * Vector3f::Dot(incidentVec, normal) * normal;
    }

    const Vector3f Refract(const Vector3f &incidentVec, const Vector3f &normal, float eta)
    {
        float N_dot_I = Vector3f::Dot(normal, incidentVec);
        float k = 1.f - eta * eta * (1.f - N_dot_I * N_dot_I);
        if (k < 0.f)
            return Vector3f(0.f, 0.f, 0.f);
        else
            return eta * incidentVec - (eta * N_dot_I + sqrtf(k)) * normal;
    }
}
