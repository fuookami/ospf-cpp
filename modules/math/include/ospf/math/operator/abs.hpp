#pragma once
/// 运算符 - 绝对值 / Operator - absolute value /// 1:1 对应 Rust operator/abs.rs
#include <cmath>
namespace ospf::math::op {
    template<typename T> [[nodiscard]] constexpr T abs(T v) noexcept { return v < T{0} ? -v : v; }
}
