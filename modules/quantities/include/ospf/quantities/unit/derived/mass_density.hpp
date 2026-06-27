#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_MASS_DENSITY = {1, -3, 0, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_PER_CUBIC_METER = {DIM_MASS_DENSITY, 1.0, "kilogram per cubic meter", "kg/m3"};
    inline const Unit UNIT_GRAM_PER_CUBIC_CENTIMETER = {DIM_MASS_DENSITY, 1000.0, "gram per cubic centimeter", "g/cm3"};
}