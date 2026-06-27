#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_ELECTRIC_CHARGE = {0, 0, 1, 1, 0, 0, 0};
    inline constexpr Dimension DIM_VOLTAGE = {1, 2, -3, -1, 0, 0, 0};
    inline constexpr Dimension DIM_CAPACITANCE = {-1, -2, 4, 2, 0, 0, 0};
    inline constexpr Dimension DIM_INDUCTANCE = {1, 2, -2, -2, 0, 0, 0};
    inline const Unit UNIT_COULOMB = {DIM_ELECTRIC_CHARGE, 1.0, "coulomb", "C"};
    inline const Unit UNIT_VOLT = {DIM_VOLTAGE, 1.0, "volt", "V"};
    inline const Unit UNIT_KILOVOLT = {DIM_VOLTAGE, 1000.0, "kilovolt", "kV"};
    inline const Unit UNIT_FARAD = {DIM_CAPACITANCE, 1.0, "farad", "F"};
    inline const Unit UNIT_HENRY = {DIM_INDUCTANCE, 1.0, "henry", "H"};
}