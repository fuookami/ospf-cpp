#pragma once
/// 包含检测 / Contains check /// 1:1 对应 Rust operator/contains.rs
namespace ospf::math::op {
    template<typename T>
    [[nodiscard]] constexpr bool in_range(T v, T lo, T hi) noexcept { return v >= lo && v <= hi; }
    template<typename T>
    [[nodiscard]] constexpr bool in_open_range(T v, T lo, T hi) noexcept { return v > lo && v < hi; }
}
