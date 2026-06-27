#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_SURFACE_DENSITY = {1, -2, 0, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_PER_SQUARE_METER = {DIM_SURFACE_DENSITY, 1.0, "kilogram per square meter", "kg/m2"};
}