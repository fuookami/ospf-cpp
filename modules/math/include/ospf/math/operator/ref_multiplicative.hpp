#pragma once
/// 乘法引用运算 / Multiplicative reference ops /// 1:1 对应 Rust operator/ref_multiplicative.rs
namespace ospf::math::op {
    template<typename T> [[nodiscard]] constexpr T mul(T a, T b) noexcept { return a * b; }
    template<typename T> [[nodiscard]] T div(T a, T b) { return a / b; }
}
