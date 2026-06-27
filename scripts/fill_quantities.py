#!/usr/bin/env python3
import os

base = "E:/workspace/ospf/ospf-cpp/modules/quantities/include/ospf/quantities/unit/derived"

files = {
    "acceleration.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_METER_PER_SECOND_SQUARED = {DIM_ACCELERATION, 1.0, "meter per second squared", "m/s2"};
    inline const Unit UNIT_STANDARD_GRAVITY = {DIM_ACCELERATION, 9.80665, "standard gravity", "g"};
}""",
    "amount_of_substance.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_MILLIMOLE = {DIM_AMOUNT, 0.001, "millimole", "mmol"};
}""",
    "angular_velocity.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_ANGULAR_VELOCITY = {0, 0, -1, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN_PER_SECOND = {DIM_ANGULAR_VELOCITY, 1.0, "radian per second", "rad/s"};
    inline const Unit UNIT_DEGREE_PER_SECOND = {DIM_ANGULAR_VELOCITY, 0.0174533, "degree per second", "deg/s"};
    inline const Unit UNIT_REVOLUTION_PER_MINUTE = {DIM_ANGULAR_VELOCITY, 0.10472, "revolution per minute", "rpm"};
}""",
    "angular_acceleration.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_ANGULAR_ACCELERATION = {0, 0, -2, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN_PER_SECOND_SQUARED = {DIM_ANGULAR_ACCELERATION, 1.0, "radian per second squared", "rad/s2"};
}""",
    "bandwidth.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_BANDWIDTH = {0, 0, -1, 0, 0, 0, 0};
    inline const Unit UNIT_BIT_PER_SECOND = {DIM_BANDWIDTH, 1.0, "bit per second", "bps"};
    inline const Unit UNIT_MEGABIT_PER_SECOND = {DIM_BANDWIDTH, 1e6, "megabit per second", "Mbps"};
}""",
    "catalytic_activity.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_CATALYTIC_ACTIVITY = {0, 0, -1, 0, 0, 1, 0};
    inline const Unit UNIT_KATAL = {DIM_CATALYTIC_ACTIVITY, 1.0, "katal", "kat"};
}""",
    "electrical.hpp": """#pragma once
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
}""",
    "flow_rate.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_FLOW_RATE = {0, 3, -1, 0, 0, 0, 0};
    inline const Unit UNIT_CUBIC_METER_PER_SECOND = {DIM_FLOW_RATE, 1.0, "cubic meter per second", "m3/s"};
    inline const Unit UNIT_LITER_PER_SECOND = {DIM_FLOW_RATE, 0.001, "liter per second", "L/s"};
}""",
    "information.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_INFORMATION = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_BIT = {DIM_INFORMATION, 1.0, "bit", "b"};
    inline const Unit UNIT_BYTE = {DIM_INFORMATION, 8.0, "byte", "B"};
    inline const Unit UNIT_KILOBYTE = {DIM_INFORMATION, 8192.0, "kilobyte", "KB"};
    inline const Unit UNIT_MEGABYTE = {DIM_INFORMATION, 8388608.0, "megabyte", "MB"};
    inline const Unit UNIT_GIGABYTE = {DIM_INFORMATION, 8589934592.0, "gigabyte", "GB"};
}""",
    "luminous_intensity.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_MILLICANDELA = {DIM_LUMINOSITY, 0.001, "millicandela", "mcd"};
}""",
    "mass_density.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_MASS_DENSITY = {1, -3, 0, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_PER_CUBIC_METER = {DIM_MASS_DENSITY, 1.0, "kilogram per cubic meter", "kg/m3"};
    inline const Unit UNIT_GRAM_PER_CUBIC_CENTIMETER = {DIM_MASS_DENSITY, 1000.0, "gram per cubic centimeter", "g/cm3"};
}""",
    "momentum.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_MOMENTUM = {1, 1, -1, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_METER_PER_SECOND = {DIM_MOMENTUM, 1.0, "kilogram meter per second", "kg*m/s"};
}""",
    "plane_angle.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_PLANE_ANGLE = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_RADIAN = {DIM_PLANE_ANGLE, 1.0, "radian", "rad"};
    inline const Unit UNIT_DEGREE = {DIM_PLANE_ANGLE, 0.017453292519943295, "degree", "deg"};
    inline const Unit UNIT_GRADIAN = {DIM_PLANE_ANGLE, 0.015707963267948967, "gradian", "grad"};
}""",
    "resistance.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_RESISTANCE = {1, 2, -3, -2, 0, 0, 0};
    inline const Unit UNIT_OHM = {DIM_RESISTANCE, 1.0, "ohm", "ohm"};
    inline const Unit UNIT_KILOHM = {DIM_RESISTANCE, 1000.0, "kilohm", "kohm"};
    inline const Unit UNIT_MEGOHM = {DIM_RESISTANCE, 1e6, "megohm", "Mohm"};
}""",
    "solid_angle.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_SOLID_ANGLE = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_STERADIAN = {DIM_SOLID_ANGLE, 1.0, "steradian", "sr"};
}""",
    "stress.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_STRESS = {1, -1, -2, 0, 0, 0, 0};
    inline const Unit UNIT_PASCAL = {DIM_STRESS, 1.0, "pascal", "Pa"};
    inline const Unit UNIT_MEGAPASCAL = {DIM_STRESS, 1e6, "megapascal", "MPa"};
}""",
    "surface_density.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_SURFACE_DENSITY = {1, -2, 0, 0, 0, 0, 0};
    inline const Unit UNIT_KILOGRAM_PER_SQUARE_METER = {DIM_SURFACE_DENSITY, 1.0, "kilogram per square meter", "kg/m2"};
}""",
    "thermodynamic_temperature.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_CELSIUS = {DIM_TEMPERATURE, 1.0, "celsius", "C"};
    inline const Unit UNIT_FAHRENHEIT = {DIM_TEMPERATURE, 5.0/9.0, "fahrenheit", "F"};
    inline const Unit UNIT_RANKINE = {DIM_TEMPERATURE, 5.0/9.0, "rankine", "R"};
}""",
    "torque.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_TORQUE = {1, 2, -2, 0, 0, 0, 0};
    inline const Unit UNIT_NEWTON_METER = {DIM_TORQUE, 1.0, "newton meter", "N*m"};
    inline const Unit UNIT_FOOT_POUND = {DIM_TORQUE, 1.3558179483314, "foot pound", "ft*lb"};
}""",
    "wavenumber.hpp": """#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_WAVENUMBER = {0, -1, 0, 0, 0, 0, 0};
    inline const Unit UNIT_RECIPROCAL_METER = {DIM_WAVENUMBER, 1.0, "reciprocal meter", "1/m"};
    inline const Unit UNIT_RECIPROCAL_CENTIMETER = {DIM_WAVENUMBER, 100.0, "reciprocal centimeter", "1/cm"};
}""",
    "macros.hpp": """#pragma once
/// Unit macros
""",
}

for name, content in files.items():
    path = os.path.join(base, name)
    with open(path, 'w') as f:
        f.write(content)

print(f"Filled {len(files)} quantities unit/derived files")
