#pragma once
/// Pressure unit /// 1:1 ∂‘”¶ Rust quantities/unit/derived/pressure.rs
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_KILOPASCAL = {DIM_PRESSURE, 1000.0, "kilopascal", "kPa"};
    inline const Unit UNIT_BAR = {DIM_PRESSURE, 100000.0, "bar", "bar"};
    inline const Unit UNIT_ATMOSPHERE = {DIM_PRESSURE, 101325.0, "atmosphere", "atm"};
    inline const Unit UNIT_PSI = {DIM_PRESSURE, 6894.76, "pounds per square inch", "psi"};
}