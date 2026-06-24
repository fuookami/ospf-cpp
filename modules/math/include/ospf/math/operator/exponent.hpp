#pragma once
/// 幂运算 / Exponent /// 1:1 对应 Rust operator/exponent.rs
#include <cmath>
namespace ospf::math::op {
    template<typename T> [[nodiscard]] constexpr T square(T v) noexcept { return v * v; }
    template<typename T> [[nodiscard]] constexpr T cube(T v) noexcept { return v * v * v; }
    template<typename T> [[nodiscard]] T pow(T base, T exp) { return std::pow(base, exp); }
    template<typename T> [[nodiscard]] T sqrt(T v) { return std::sqrt(v); }
}
