#define CATCH_CONFIG_MAIN
#include "../lib/Catch2/catch.hpp"
#include "../src/mathfunc.h"

#include <array>

using namespace OpenPT;

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    REQUIRE(true);
}

TEST_CASE("Vector initialization", "[Vector3f]")
{
    std::array<float, 3> initializer = {0.5f, 0.6f, 0.7f};
    Vector3f a = initializer;
    SECTION("Initialization check")
    {
        REQUIRE((a.x == 0.5f && a.y == 0.6f && a.z == 0.7f));
    }
    SECTION("Bracket operator check")
    {
        for(int i=0;i<3;++i){
            INFO("Current i: "<<i);
            INFO("Current value of Vector3f: "<<a[i]);
            REQUIRE(initializer[i] == a[i]);
        }
    }
    // REQUIRE(true);
}