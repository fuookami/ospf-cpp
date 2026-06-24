#pragma once
/// 容差比较 / Tolerance comparison /// 1:1 对应 Rust operator/tolerance.rs
#include <cmath>
#include <limits>
namespace ospf::math::op {
    template<typename T>
    [[nodiscard]] bool approx_equal(T a, T b, T eps = std::numeric_limits<T>::epsilon()) {
        if (a == b) return true;
        T diff = std::abs(a - b);
        T largest = std::max(std::abs(a), std::abs(b));
        return diff <= largest * eps;
    }
    template<typename T>
    [[nodiscard]] bool approx_zero(T v, T eps = std::numeric_limits<T>::epsilon()) {
        return std::abs(v) <= eps;
    }
}
