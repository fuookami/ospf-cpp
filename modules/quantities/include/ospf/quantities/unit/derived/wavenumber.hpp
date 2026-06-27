#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_WAVENUMBER = {0, -1, 0, 0, 0, 0, 0};
    inline const Unit UNIT_RECIPROCAL_METER = {DIM_WAVENUMBER, 1.0, "reciprocal meter", "1/m"};
    inline const Unit UNIT_RECIPROCAL_CENTIMETER = {DIM_WAVENUMBER, 100.0, "reciprocal centimeter", "1/cm"};
}