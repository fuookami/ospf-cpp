#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_SOLID_ANGLE = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_STERADIAN = {DIM_SOLID_ANGLE, 1.0, "steradian", "sr"};
}