/*
 *  OpenPT - Math Function Unit Test
 *  File created on 2023/1/10
 *  Last edited on 2023/1/10
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#define CATCH_CONFIG_MAIN
#include "../lib/Catch2/catch.hpp"
#include "../src/mathfunc.h"

#include <array>
#include <cmath>

using namespace OpenPT;

std::array<float, 3> initializer = {0.5f, 0.6f, 0.7f};

TEST_CASE("Testing framework is working fine")
{
    REQUIRE(true);
}

TEST_CASE("Vector3f initialization")
{
    Vector3f a = initializer;
    SECTION("Initialization check")
    {
        REQUIRE((a.x == 0.5f && a.y == 0.6f && a.z == 0.7f));
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
}

TEST_CASE("Vector3f value-modifying test")
{
    Vector3f a = initializer;
    a[0] += 0.1f;
    REQUIRE(a.x == initializer[0] + 0.1f);
    a[1] += 0.1f;
    REQUIRE(a.y == initializer[1] + 0.1f);
    a[2] += 0.1f;
    REQUIRE(a.z == initializer[2] + 0.1f);

    a.x -= 0.1f;
    REQUIRE(a[0] == initializer[0]);
    a.y -= 0.1f;
    REQUIRE(a[1] == initializer[1]);
    a.z -= 0.1f;
    REQUIRE(a[2] == initializer[2]);
}

TEST_CASE("Vector3f operation test")
{
    SECTION("Normalize")
    {
        Vector3f a = initializer;
        a.Normalize();
        REQUIRE(std::abs(a.Length() - 1.0f) < EPS);
        Vector3f b = a.Normalized();
        REQUIRE(std::abs((a - b).Length()) < EPS);
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
        Vector3f a = {0.2, 0.3, 0.4};
        Vector3f b = {0.5, 0.7, 0.1};
        auto c = Vector3f::Cross(a, b);
        REQUIRE(std::abs(Vector3f::Dot(a, c)) < EPS);
        REQUIRE(std::abs(Vector3f::Dot(b, c)) < EPS);
    }
}

TEST_CASE("Vector3f security")
{
    SECTION("Index overflow")
    {
        Vector3f a = initializer;
        a[999999] = 10.0f;
    }
}

TEST_CASE("Vector constant test")
{
    SECTION("Vector3f")
    {
        Vector3f ret = Vector3f::O + Vector3f::X + Vector3f::Y + Vector3f::Z;
        REQUIRE(ret == Vector3f(1.0f, 1.0f, 1.0f));
    }
}

TEST_CASE("Vector test")
{
    Vector4f a = {1.0f, 2.0f, 3.0f, 4.0f};
    a = -a;
    REQUIRE(a[0] == -1.0f);
}

TEST_CASE("Matrix4x4f test")
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
}