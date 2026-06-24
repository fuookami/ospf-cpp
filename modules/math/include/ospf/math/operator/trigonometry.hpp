#pragma once
/// 三角函数 / Trigonometry /// 1:1 对应 Rust operator/trigonometry.rs
#include <cmath>
#include <numbers>
namespace ospf::math::op {
    template<typename T> [[nodiscard]] T sin_deg(T d) { return std::sin(d * std::numbers::pi_v<T> / T{180}); }
    template<typename T> [[nodiscard]] T cos_deg(T d) { return std::cos(d * std::numbers::pi_v<T> / T{180}); }
    template<typename T> [[nodiscard]] T tan_deg(T d) { return std::tan(d * std::numbers::pi_v<T> / T{180}); }
}
