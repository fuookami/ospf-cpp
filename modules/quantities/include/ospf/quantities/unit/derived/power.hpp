#pragma once
/// Power unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/power.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILOWATT = {DIM_POWER, 1000.0, "kilowatt", "kW"};
    inline const Unit UNIT_HORSEPOWER = {DIM_POWER, 745.7, "horsepower", "hp"};
}