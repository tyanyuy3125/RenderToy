/*
 *  RenderToy - Importer Unit Test
 *  File created on 2023/1/11
 *  Last edited on 2023/1/11
 *  Tianyu Huang <tianyu@illumiart.net>
 */

#define CATCH_CONFIG_MAIN

#include "../include/RenderToy/rendertoy.h"
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

TEST_CASE("Triangle")
{
    std::array<Vector3f, 3> vert = {Vector3f(-1.0f, -1.0f, 0.0f),Vector3f(-1.0f, -1.0f, 2.0f),Vector3f(1.0f, -1.0f, 2.0f)};
    std::array<Vector3f, 3> norm = {Vector3f::O,Vector3f::O,Vector3f::O};
    std::array<Vector2f, 3> uv = {Vector2f::O,Vector2f::O};
    Triangle triangle(vert, norm, uv, nullptr);
    REQUIRE(std::abs(triangle.NormalC().Cross({0.0f, 1.0f, 0.0f}).Length())<kFloatEpsilon);
}