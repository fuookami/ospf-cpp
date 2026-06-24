#pragma once
/// 零值和单位值引用 / Zero and one reference /// 1:1 对应 Rust operator/one_zero_ref.rs
namespace ospf::math::op {
    template<typename T> [[nodiscard]] constexpr T zero() noexcept { return T{0}; }
    template<typename T> [[nodiscard]] constexpr T one() noexcept { return T{1}; }
}
