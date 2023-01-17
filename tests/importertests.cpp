/*
 *  OpenPT - Importer Unit Test
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#define CATCH_CONFIG_MAIN
#include "../lib/Catch2/catch.hpp"
#include "../src/mathfunc.h"
#include "../src/importer.h"
#include "../src/meshobj.h"

#include <sstream>
#include <string>
#include <cmath>

using namespace OpenPT;

TEST_CASE("SStream Functionality Test")
{
    SECTION("Vertex")
    {
        std::string str = "v 1.000000 1.000000 -1.000000";
        std::istringstream str_iss(str);
        std::string identifier;
        str_iss >> identifier;
        float x, y, z;
        str_iss >> x >> y >> z;
        REQUIRE(identifier == "v");
        REQUIRE(std::abs(x - 1.0f) < EPS);
        REQUIRE(std::abs(y - 1.0f) < EPS);
        REQUIRE(std::abs(z + 1.0f) < EPS);
    }
    SECTION("Face definition")
    {
        std::string str = "5/4/1";
        std::istringstream str_iss(str);
        int a, b, c;
        char slash;
        str_iss >> a >> slash >> b >> slash >> c;
        REQUIRE(a == 5);
        REQUIRE(b == 4);
        REQUIRE(c == 1);
    }
}

TEST_CASE("Importer"){
    Importer importer("./cube.obj");
    
    REQUIRE(importer.Import().size() == 1);
}
