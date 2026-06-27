#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_RESISTANCE = {1, 2, -3, -2, 0, 0, 0};
    inline const Unit UNIT_OHM = {DIM_RESISTANCE, 1.0, "ohm", "ohm"};
    inline const Unit UNIT_KILOHM = {DIM_RESISTANCE, 1000.0, "kilohm", "kohm"};
    inline const Unit UNIT_MEGOHM = {DIM_RESISTANCE, 1e6, "megohm", "Mohm"};
}