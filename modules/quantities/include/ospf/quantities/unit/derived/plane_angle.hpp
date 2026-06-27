#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_PLANE_ANGLE = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN = {DIM_PLANE_ANGLE, 1.0, "radian", "rad"};
    inline const Unit UNIT_DEGREE = {DIM_PLANE_ANGLE, 0.017453292519943295, "degree", "deg"};
    inline const Unit UNIT_GRADIAN = {DIM_PLANE_ANGLE, 0.015707963267948967, "gradian", "grad"};
}