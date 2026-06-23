#pragma once

/// 运算符重载概念与工具 / Operator overload concepts and utilities
/// 提供容差比较、包含检测、距离计算等。

#include <cmath>
#include <concepts>

namespace ospf::math {

    // -- 容差比较 / Tolerance comparison --------------------------------------

    /// 检查两个值是否在容差范围内相等 / Check if two values are approximately equal
    template<typename T>
    [[nodiscard]] bool approx_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
        if (a == b) return true;
        T diff = std::abs(a - b);
        T largest = std::max(std::abs(a), std::abs(b));
        return diff <= largest * epsilon;
    }

    /// 检查值是否接近零 / Check if value is approximately zero
    template<typename T>
    [[nodiscard]] bool approx_zero(T value, T epsilon = std::numeric_limits<T>::epsilon()) {
        return std::abs(value) <= epsilon;
    }

    // -- 包含检测 / Contains --------------------------------------------------

    /// 检查值是否在范围内（闭区间） / Check if value is in range (closed interval)
    template<typename T>
    [[nodiscard]] constexpr bool in_range(T value, T lower, T upper) noexcept {
        return value >= lower && value <= upper;
    }

    /// 检查值是否在范围内（开区间） / Check if value is in range (open interval)
    template<typename T>
    [[nodiscard]] constexpr bool in_open_range(T value, T lower, T upper) noexcept {
        return value > lower && value < upper;
    }

    // -- 距离 / Distance ------------------------------------------------------

    /// 一维距离 / 1D distance
    template<typename T>
    [[nodiscard]] constexpr T distance_1d(T a, T b) noexcept {
        return std::abs(a - b);
    }

    // -- 插值 / Interpolation -------------------------------------------------

    /// 线性插值 / Linear interpolation (lerp)
    template<typename T>
    [[nodiscard]] constexpr T lerp(T a, T b, T t) noexcept {
        return a + t * (b - a);
    }

    // -- 钳制 / Clamping ------------------------------------------------------

    /// 钳制值到范围 / Clamp value to range
    template<typename T>
    [[nodiscard]] constexpr T clamp(T value, T lower, T upper) noexcept {
        if (value < lower) return lower;
        if (value > upper) return upper;
        return value;
    }

    // -- 符号 / Sign ----------------------------------------------------------

    /// 符号函数 / Sign function
    template<typename T>
    [[nodiscard]] constexpr int sign(T value) noexcept {
        if (value > T{0}) return 1;
        if (value < T{0}) return -1;
        return 0;
    }

}  // namespace ospf::math
