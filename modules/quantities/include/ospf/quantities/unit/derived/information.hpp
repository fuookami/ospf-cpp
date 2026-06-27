#pragma once
#include <ospf/quantities/unit.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_INFORMATION = {0, 0, 0, 0, 0, 0, 0};
    inline const Unit UNIT_BIT = {DIM_INFORMATION, 1.0, "bit", "b"};
    inline const Unit UNIT_BYTE = {DIM_INFORMATION, 8.0, "byte", "B"};
    inline const Unit UNIT_KILOBYTE = {DIM_INFORMATION, 8192.0, "kilobyte", "KB"};
    inline const Unit UNIT_MEGABYTE = {DIM_INFORMATION, 8388608.0, "megabyte", "MB"};
    inline const Unit UNIT_GIGABYTE = {DIM_INFORMATION, 8589934592.0, "gigabyte", "GB"};
}