#define CATCH_CONFIG_MAIN
#include "../lib/Catch2/catch.hpp"
#include "../src/mathfunc.h"

#include <array>
#include <cmath>

using namespace OpenPT;

std::array<float, 3> initializer = {0.5f, 0.6f, 0.7f};

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    REQUIRE(true);
}

TEST_CASE("Vector initialization", "[Vector3f]")
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

TEST_CASE("Vector value-modifying test", "[Vector3f]")
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

TEST_CASE("Vector operation test", "[Vector3f]")
{
    SECTION("Normalize")
    {
        Vector3f a = initializer;
        a.Normalize();
        REQUIRE(std::abs(a.Length() - 1.0f) < EPS);
    }

    SECTION("Equal"){
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

    SECTION("Dot product and cross product"){
        Vector3f a = {0.2,0.3,0.4};
        Vector3f b = {0.5,0.7,0.1};
        auto c = Vector3f::Cross(a,b);
        REQUIRE(std::abs(Vector3f::Dot(a,c))<EPS);
        REQUIRE(std::abs(Vector3f::Dot(b,c))<EPS);
    }
}