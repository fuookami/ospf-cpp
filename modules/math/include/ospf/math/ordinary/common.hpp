#pragma once
/// 普通数学函数 - 通用 / Common ordinary functions /// 1:1 对应 Rust ordinary/common.rs
#include <cmath>
namespace ospf::math::ordinary {
    template<typename T> [[nodiscard]] constexpr T abs(T v) noexcept { return v < T{0} ? -v : v; }
    template<typename T> [[nodiscard]] T reciprocal(T v) { return T{1} / v; }
    template<typename T> [[nodiscard]] constexpr T square(T v) noexcept { return v * v; }
    template<typename T> [[nodiscard]] constexpr T cube(T v) noexcept { return v * v * v; }
}
