#pragma once
/// Velocity unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/velocity.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILOMETER_PER_HOUR = {DIM_VELOCITY, 1.0/3.6, "kilometer per hour", "km/h"};
    inline const Unit UNIT_MILE_PER_HOUR = {DIM_VELOCITY, 0.44704, "mile per hour", "mph"};
    inline const Unit UNIT_KNOT = {DIM_VELOCITY, 0.514444, "knot", "kn"};
}