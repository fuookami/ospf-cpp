#pragma once
/// 最小公倍数 / LCM /// 1:1 对应 Rust ordinary/lcm.rs
#include <ospf/math/ordinary/gcd.hpp>
namespace ospf::math::ordinary {
    template<typename T>
    [[nodiscard]] constexpr T lcm(T a, T b) noexcept { return a / gcd(a, b) * b; }
}
