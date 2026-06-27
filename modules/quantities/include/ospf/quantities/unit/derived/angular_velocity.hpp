#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_ANGULAR_VELOCITY = {0, 0, -1, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN_PER_SECOND = {DIM_ANGULAR_VELOCITY, 1.0, "radian per second", "rad/s"};
    inline const Unit UNIT_DEGREE_PER_SECOND = {DIM_ANGULAR_VELOCITY, 0.0174533, "degree per second", "deg/s"};
    inline const Unit UNIT_REVOLUTION_PER_MINUTE = {DIM_ANGULAR_VELOCITY, 0.10472, "revolution per minute", "rpm"};
}