#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include "../src/rtmath.h"

#include <array>
#include <cmath>

using namespace RenderToy;

Vector3f initializer = {0.5f, 0.6f, 0.7f};

TEST_CASE("Catch2")
{
    REQUIRE(true);
}

TEST_CASE("Vector")
{
    Vector3f hanging_vector;
    Vector3f a = initializer;
    SECTION("Initialization check")
    {
        REQUIRE((a.x() == 0.5f && a.y() == 0.6f && a.z() == 0.7f));
    }
    SECTION("Bracket operator check")
    {
        for (int i = 0; i < 3; ++i)
        {
            INFO("Current i: " << i);
            INFO("Current value of Vector3f: " << a[i]);
            REQUIRE(initializer[i] == a[i]);
        }
    }

    SECTION("Vector3f value-modifying test")
    {
        Vector3f a = initializer;
        a[0] += 0.1f;
        REQUIRE(a.x() == initializer[0] + 0.1f);
        a[1] += 0.1f;
        REQUIRE(a.y() == initializer[1] + 0.1f);
        a[2] += 0.1f;
        REQUIRE(a.z() == initializer[2] + 0.1f);

        a.x() -= 0.1f;
        REQUIRE(a[0] == initializer[0]);
        a.y() -= 0.1f;
        REQUIRE(a[1] == initializer[1]);
        a.z() -= 0.1f;
        REQUIRE(a[2] == initializer[2]);
    }

    SECTION("Normalize")
    {
        Vector3f a = initializer;
        a.Normalize();
        // REQUIRE(std::abs(a.Length() - 1.0f) < kFloatEpsilon);
        REQUIRE(a.Length() == 1.0f);
        Vector3f b = a.Normalized();
        // REQUIRE(std::abs((a - b).Length()) < kFloatEpsilon);
        REQUIRE((a - b).Length() == 0.0f);
    }

    SECTION("Equal")
    {
        Vector3f a = initializer;
        Vector3f b = initializer;
        REQUIRE(a == b);
    }

    SECTION("Adding and Multiplication")
    {
        Vector3f a = initializer;
        a *= 2.0f;
        Vector3f b = initializer;
        REQUIRE(a == b * 2.0f);
        a += a;
        b += 3.0f * b;
        REQUIRE(a == b);
    }

    SECTION("Minus and Division")
    {
        Vector3f a = initializer;
        Vector3f b = initializer;
        a -= {0.1f, 0.2f, 0.3f};
        b -= {0.2f, 0.4f, 0.6f};
        REQUIRE((b - Vector3f(0.1f, 0.2f, 0.3f) == a - Vector3f(0.2f, 0.4f, 0.6f)));
    }

    SECTION("Dot product and cross product")
    {
        Vector3f a = {0.2f, 0.3f, 0.4f};
        Vector3f b = {0.5f, 0.7f, 0.1f};
        auto c = Vector3f::Cross(a, b);
        REQUIRE(std::abs(Vector3f::Dot(a, c)) < kFloatEpsilon);
        REQUIRE(std::abs(Vector3f::Dot(b, c)) < kFloatEpsilon);
        // REQUIRE(Vector3f::Dot(a, c) == 0.0f);
        // REQUIRE(Vector3f::Dot(b, c) == 0.0f);
    }

    Vector3f ret = Vector3f::O + Vector3f::X + Vector3f::Y + Vector3f::Z;
    REQUIRE(ret == Vector3f(1.0f, 1.0f, 1.0f));

    SECTION("Negativity")
    {
        Vector4f a = {1.0f, 2.0f, 3.0f, 4.0f};
        a = -a;
        REQUIRE(a[0] == -1.0f);
    }

    SECTION("Per-component transformation")
    {
        Vector2d a = {1.14514, 1.919810};
        auto b = a.Floor();
        REQUIRE(b[0] == 1.0);
        auto c = a.Fract();
        REQUIRE(c[1] == 0.919810);
        auto d = a.Sin();
        REQUIRE(std::abs(d[0] - 0.9107) < 1e-4);
    }
}

TEST_CASE("Matrix")
{
    Matrix4x4f a = Matrix4x4f::I;
    Vector4f vec1 = {1.0f, 2.0f, 3.0f, 4.0f};
    Vector4f vec2 = vec1;
    REQUIRE(a * vec1 == vec2);

    Matrix4x4f b = {{0.0f, 0.0f, 0.0f, 1.0f},
                    {0.0f, 0.0f, 1.0f, 0.0f},
                    {0.0f, 1.0f, 0.0f, 0.0f},
                    {1.0f, 0.0f, 0.0f, 0.0f}};
    REQUIRE(b * b == Matrix4x4f::I);
    REQUIRE(b * vec1 == (Vector4f){4.0f, 3.0f, 2.0f, 1.0f});

    SECTION("Determinant")
    {
        float det = Matrix3x3f::Determinant((Matrix3x3f){{1.0f, 2.0f, 3.0f},
                                                         {4.0f, 5.0f, 6.0f},
                                                         {7.0f, 8.0f, 9.0f}});
        // REQUIRE(std::abs(det - 0.0f) < kFloatEpsilon);
        REQUIRE(det == 0.0f);

        det = Matrix3x3f::Determinant((Matrix3x3f){{1.0f, 2.0f, 3.0f},
                                                   {4.0f, 5.0f, 6.0f},
                                                   {15.0f, 8.0f, 9.0f}});
        // REQUIRE(std::abs(det + 24.0f) < kFloatEpsilon);
        REQUIRE(det == -24.0f);
    }

    SECTION("ComplementMinor")
    {
        Matrix4x4f b = {{0.0f, 0.0f, 0.0f, 1.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {1.0f, 0.0f, 0.0f, 0.0f}};
        Matrix3x3f minor = {{0.0f, 1.0f, 0.0f},
                            {1.0f, 0.0f, 0.0f},
                            {0.0f, 0.0f, 0.0f}};
        REQUIRE(b.ComplementMinor(0, 0) == minor);
    }
}

TEST_CASE("SSE InvSqrt")
{
    REQUIRE(std::abs(SSE_InvSqrt(2.0f) - (1.0f / sqrt(2.0f))) < 1e-4); // SSE InvSqrt has low accuracy.
    // Fails at 1e-5.
}

TEST_CASE("Vector constexpr")
{
    constexpr Vector3d vec_a({2.0, 5.0, 4.0});
    constexpr Vector3d vec_b({3.0, 4.0, 6.0});
    constexpr double t = vec_a[1];
    constexpr double t2 = vec_a.x();
    STATIC_REQUIRE(t == 5.0);
    STATIC_REQUIRE(t2 == 2.0);
    constexpr Vector<float, 3> vec_c(1.0f, 2.0f, 3.0f);
    constexpr Vector<float, 3> vec_d = {1.0f, 2.0f, 3.0f};
    constexpr Vector3d vec_e = {3.0, 4.0, 1.0};
    constexpr Vector4d vec_f = {1.0, 2.0, 3.0, 4.0};
    constexpr double t3 = vec_a.Cross(vec_b)[0];
    STATIC_REQUIRE(t3 == 14.0);
    Vector3d vec_g(1.0);
    constexpr Vector4d vec_h = Vector4d::O;
    REQUIRE(Vector3d::X + Vector3d::Y + Vector3d::Z == Vector3d::White);
    REQUIRE(vec_c == vec_d);
    // REQUIRE(std::abs(vec_a.Normalized().Length() - 1.0) < kFloatEpsilon);
    REQUIRE(vec_a.Normalized().Length() == 1.0);
}

TEST_CASE("Matrix constexpr")
{
    constexpr Matrix2x2d mat_a({{1.0, 2.0}, {3.0, 4.0}});
    constexpr Matrix2x2d mat_b = {{1.0, 2.0},
                                  {3.0, 4.0}};
    STATIC_REQUIRE(mat_a[0][0] == mat_b[0][0]);
    REQUIRE(mat_a == mat_b);
    STATIC_REQUIRE(Matrix2x2d::Determinant(mat_a) == Matrix2x2d::Determinant(mat_b));
}

TEST_CASE("Converter")
{
    // STATIC_REQUIRE(std::abs(Convert::InchToMM(1.0f) - 25.4f) < kFloatEpsilon);
    STATIC_REQUIRE(Convert::InchToMM(1.0f) == 25.4f);
    // STATIC_REQUIRE(std::abs(Convert::DegreeToRadians(180.0f) - kPi<float>) < kFloatEpsilon);
    STATIC_REQUIRE(Convert::DegreeToRadians(180.0f) == kPi<float>);
    // STATIC_REQUIRE(std::abs(Convert::Luma({1.0f, 1.0f, 1.0f}) - (0.212671f + 0.715160f + 0.072169f)) < kFloatEpsilon);
    STATIC_REQUIRE(Convert::Luma({1.0f, 1.0f, 1.0f}) == (0.212671f + 0.715160f + 0.072169f));
}

TEST_CASE("GeneralizedVector")
{
    GeneralizedVector<int> gvec(1, 1, 5, 1, 1, 1);
    REQUIRE(gvec.column == 6);
    REQUIRE(gvec[2] == 5);

    GeneralizedVector<int> gvec2({1, 9, 1, 9, 8, 1, 0});
    REQUIRE(gvec2.column == 7);
    REQUIRE(gvec2[1] == 9);

    GeneralizedVector<int> gvec3 = {1, 1, 4, 5, 1, 4};
    REQUIRE(gvec3.column == 6);
    REQUIRE(gvec3[2] == 4);
}

TEST_CASE("GeneralizedMatrix")
{
    GeneralizedMatrix<int> gmat(10, 10);
    gmat[5][5] = 16;
    REQUIRE(gmat[5][5] == 16);

    GeneralizedMatrix<int> gmat2({{1, 2, 3, 4, 5, 6}, {1, 1, 4, 5, 1, 4}});
    REQUIRE(gmat2.row == 2);
    REQUIRE(gmat2.column == 6);
    REQUIRE(gmat2[1][2] == 4);
}

TEST_CASE("UnitizedGaussianKernel")
{
    UnitizedGaussianKernel<double> knl(3, 0.8);
    REQUIRE(std::abs(knl.kernel_mat[1][1] - 0.2725) < 0.0001);
}

TEST_CASE("PrewittKernel")
{
    PrewittKernel<int, Orientation::X> knl;
    REQUIRE(knl.kernel_mat[0][0] == -1);
    REQUIRE(knl.kernel_mat[0][2] == 1);
}

TEST_CASE("SobelKernel")
{
    SobelKernel<int, Orientation::Y> knl;
    REQUIRE(knl.kernel_mat[0][0] == 1);
    REQUIRE(knl.kernel_mat[0][2] == 1);
    REQUIRE(knl.kernel_mat[2][1] == -2);
}