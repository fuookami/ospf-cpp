#pragma once
/// Area unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/area.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_HECTARE = {DIM_AREA, 10000.0, "hectare", "ha"};
    inline const Unit UNIT_ACRE = {DIM_AREA, 4046.86, "acre", "ac"};
    inline const Unit UNIT_SQUARE_KILOMETER = {DIM_AREA, 1e6, "square kilometer", "km2"};
    inline const Unit UNIT_SQUARE_CENTIMETER = {DIM_AREA, 1e-4, "square centimeter", "cm2"};
}