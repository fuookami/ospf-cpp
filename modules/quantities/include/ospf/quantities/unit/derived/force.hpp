#pragma once
/// Force unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/force.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILONEWTON = {DIM_FORCE, 1000.0, "kilonewton", "kN"};
    inline const Unit UNIT_POUND_FORCE = {DIM_FORCE, 4.44822, "pound-force", "lbf"};
}