#pragma once
/// Time unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/time.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_MILLISECOND = {DIM_TIME, 0.001, "millisecond", "ms"};
    inline const Unit UNIT_MICROSECOND = {DIM_TIME, 1e-6, "microsecond", "us"};
    inline const Unit UNIT_DAY = {DIM_TIME, 86400.0, "day", "d"};
    inline const Unit UNIT_WEEK = {DIM_TIME, 604800.0, "week", "wk"};
    inline const Unit UNIT_YEAR = {DIM_TIME, 31557600.0, "year", "yr"};
}