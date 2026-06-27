#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_STRESS = {1, -1, -2, 0, 0, 0, 0};
    inline const Unit UNIT_PASCAL = {DIM_STRESS, 1.0, "pascal", "Pa"};
    inline const Unit UNIT_MEGAPASCAL = {DIM_STRESS, 1e6, "megapascal", "MPa"};
}