#define CATCH_CONFIG_MAIN
#include "../lib/Catch2/catch.hpp"
#include "../src/mathfunc.h"

using namespace OpenPT;

TEST_CASE("Testing framework is working fine", "[Catch2]")
{
    REQUIRE(true);
}

TEST_CASE("Vector initialization","[Vector3f]"){
    Vector3f a = {0.5f, 0.6f, 0.7f};
    REQUIRE((a.x() == 0.5f && a.y()==0.6f &&a.z()==0.7f));
    // REQUIRE(true);
}