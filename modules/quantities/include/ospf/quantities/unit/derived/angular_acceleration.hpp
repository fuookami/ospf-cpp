#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_ANGULAR_ACCELERATION = {0, 0, -2, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN_PER_SECOND_SQUARED = {DIM_ANGULAR_ACCELERATION, 1.0, "radian per second squared", "rad/s2"};
}