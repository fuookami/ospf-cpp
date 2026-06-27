#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_MOMENTUM = {1, 1, -1, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_METER_PER_SECOND = {DIM_MOMENTUM, 1.0, "kilogram meter per second", "kg*m/s"};
}