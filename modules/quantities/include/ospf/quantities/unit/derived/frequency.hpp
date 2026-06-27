#pragma once
/// Frequency unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/frequency.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILOHERTZ = {DIM_FREQUENCY, 1000.0, "kilohertz", "kHz"};
    inline const Unit UNIT_MEGAHERTZ = {DIM_FREQUENCY, 1e6, "megahertz", "MHz"};
    inline const Unit UNIT_GIGAHERTZ = {DIM_FREQUENCY, 1e9, "gigahertz", "GHz"};
}