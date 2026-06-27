#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_CATALYTIC_ACTIVITY = {0, 0, -1, 0, 0, 1, 0};
    inline const Unit UNIT_KATAL = {DIM_CATALYTIC_ACTIVITY, 1.0, "katal", "kat"};
}