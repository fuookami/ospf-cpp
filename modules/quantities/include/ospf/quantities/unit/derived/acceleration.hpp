#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_METER_PER_SECOND_SQUARED = {DIM_ACCELERATION, 1.0, "meter per second squared", "m/s2"};
    inline const Unit UNIT_STANDARD_GRAVITY = {DIM_ACCELERATION, 9.80665, "standard gravity", "g"};
}