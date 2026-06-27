#pragma once
/// Derived dimension /// 1:1 ∂‘”¶ Rust quantities/dimension/derived.rs
#include <ospf/quantities/dimension.hpp>
namespace ospf::quantities {
    inline constexpr Dimension DIM_AREA = {0, 2, 0, 0, 0, 0, 0};
    inline constexpr Dimension DIM_VOLUME = {0, 3, 0, 0, 0, 0, 0};
    inline constexpr Dimension DIM_VELOCITY = {0, 1, -1, 0, 0, 0, 0};
    inline constexpr Dimension DIM_ACCELERATION = {0, 1, -2, 0, 0, 0, 0};
    inline constexpr Dimension DIM_FORCE = {1, 1, -2, 0, 0, 0, 0};
    inline constexpr Dimension DIM_ENERGY = {1, 2, -2, 0, 0, 0, 0};
    inline constexpr Dimension DIM_POWER = {1, 2, -3, 0, 0, 0, 0};
    inline constexpr Dimension DIM_PRESSURE = {1, -1, -2, 0, 0, 0, 0};
    inline constexpr Dimension DIM_FREQUENCY = {0, 0, -1, 0, 0, 0, 0};
    inline constexpr Dimension DIM_ELECTRIC_CHARGE = {0, 0, 1, 1, 0, 0, 0};
    inline constexpr Dimension DIM_VOLTAGE = {1, 2, -3, -1, 0, 0, 0};
    inline constexpr Dimension DIM_RESISTANCE = {1, 2, -3, -2, 0, 0, 0};
    inline constexpr Dimension DIM_CAPACITANCE = {-1, -2, 4, 2, 0, 0, 0};
    inline constexpr Dimension DIM_INDUCTANCE = {1, 2, -2, -2, 0, 0, 0};
    inline constexpr Dimension DIM_LUMINOUS_FLUX = {0, 0, 0, 0, 0, 0, 1};
    inline constexpr Dimension DIM_ILLUMINANCE = {0, -2, 0, 0, 0, 0, 1};
}