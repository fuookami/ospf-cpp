#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_TORQUE = {1, 2, -2, 0, 0, 0, 0};
    inline const Unit UNIT_NEWTON_METER = {DIM_TORQUE, 1.0, "newton meter", "N*m"};
    inline const Unit UNIT_FOOT_POUND = {DIM_TORQUE, 1.3558179483314, "foot pound", "ft*lb"};
}