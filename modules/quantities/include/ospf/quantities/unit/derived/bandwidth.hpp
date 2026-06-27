#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_BANDWIDTH = {0, 0, -1, 0, 0, 0, 0};
    inline const Unit UNIT_BIT_PER_SECOND = {DIM_BANDWIDTH, 1.0, "bit per second", "bps"};
    inline const Unit UNIT_MEGABIT_PER_SECOND = {DIM_BANDWIDTH, 1e6, "megabit per second", "Mbps"};
}