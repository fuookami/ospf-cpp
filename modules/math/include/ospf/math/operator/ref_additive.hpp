#pragma once
/// 加法引用运算 / Additive reference ops /// 1:1 对应 Rust operator/ref_additive.rs
namespace ospf::math::op {
    template<typename T> [[nodiscard]] constexpr T add(T a, T b) noexcept { return a + b; }
    template<typename T> [[nodiscard]] constexpr T sub(T a, T b) noexcept { return a - b; }
    template<typename T> [[nodiscard]] constexpr T neg(T a) noexcept { return -a; }
}
