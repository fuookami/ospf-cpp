#pragma once
/// Mass unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/mass.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_MILLIGRAM = {DIM_MASS, 1e-6, "milligram", "mg"};
    inline const Unit UNIT_POUND = {DIM_MASS, 0.453592, "pound", "lb"};
    inline const Unit UNIT_OUNCE = {DIM_MASS, 0.0283495, "ounce", "oz"};
}