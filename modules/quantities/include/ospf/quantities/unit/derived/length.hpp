#pragma once
/// Length unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/length.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_INCH = {DIM_LENGTH, 0.0254, "inch", "in"};
    inline const Unit UNIT_FOOT = {DIM_LENGTH, 0.3048, "foot", "ft"};
    inline const Unit UNIT_YARD = {DIM_LENGTH, 0.9144, "yard", "yd"};
    inline const Unit UNIT_MILE = {DIM_LENGTH, 1609.34, "mile", "mi"};
}