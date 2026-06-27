#pragma once
/// Volume unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/volume.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_LITER = {DIM_VOLUME, 0.001, "liter", "L"};
    inline const Unit UNIT_MILLILITER = {DIM_VOLUME, 1e-6, "milliliter", "mL"};
    inline const Unit UNIT_GALLON = {DIM_VOLUME, 0.00378541, "gallon", "gal"};
}