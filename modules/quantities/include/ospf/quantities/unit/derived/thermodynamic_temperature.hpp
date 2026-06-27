#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline const Unit UNIT_CELSIUS = {DIM_TEMPERATURE, 1.0, "celsius", "C"};
    inline const Unit UNIT_FAHRENHEIT = {DIM_TEMPERATURE, 5.0/9.0, "fahrenheit", "F"};
    inline const Unit UNIT_RANKINE = {DIM_TEMPERATURE, 5.0/9.0, "rankine", "R"};
}