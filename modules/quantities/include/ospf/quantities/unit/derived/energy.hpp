#pragma once
/// Energy unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/energy.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILOJOULE = {DIM_ENERGY, 1000.0, "kilojoule", "kJ"};
    inline const Unit UNIT_CALORIE = {DIM_ENERGY, 4.184, "calorie", "cal"};
    inline const Unit UNIT_KILOCALORIE = {DIM_ENERGY, 4184.0, "kilocalorie", "kcal"};
    inline const Unit UNIT_KILOWATT_HOUR = {DIM_ENERGY, 3.6e6, "kilowatt-hour", "kWh"};
}