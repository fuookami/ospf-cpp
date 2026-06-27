#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_FLOW_RATE = {0, 3, -1, 0, 0, 0, 0};
    inline const Unit UNIT_CUBIC_METER_PER_SECOND = {DIM_FLOW_RATE, 1.0, "cubic meter per second", "m3/s"};
    inline const Unit UNIT_LITER_PER_SECOND = {DIM_FLOW_RATE, 0.001, "liter per second", "L/s"};
}