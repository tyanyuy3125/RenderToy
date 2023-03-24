
#define CATCH_CONFIG_MAIN

#include <RenderToy/rendertoy.h>
#include <catch2/catch_all.hpp>

using namespace RenderToy;

TEST_CASE("SetO2W Test")
{
    Geometry obj;
    REQUIRE(obj.GetO2W() == obj.GetW2O());
    REQUIRE(obj.GetW2O() == Matrix4x4f::I);

    Matrix4x4f trm = {{0.0f, 1.0f, 0.0f, 5.0f},
                      {1.0f, 0.0f, 0.0f, 23.0f},
                      {0.0f, 0.0f, 1.0f, 14.0f},
                      {0.0f, 0.0f, 0.0f, 1.0f}};

    Matrix4x4f trm_inv = {{0.0f, 1.0f, 0.0f, -23.0f},
                          {1.0f, 0.0f, 0.0f, -5.0f},
                          {0.0f, 0.0f, 1.0f, -14.0f},
                          {0.0f, 0.0f, 0.0f, 1.0f}};
    obj.SetO2W(trm);
    REQUIRE(obj.GetO2W() == trm);
    REQUIRE(obj.GetW2O() == trm_inv);
}