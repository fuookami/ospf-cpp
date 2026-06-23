#pragma once

/// 普通数学函数 / Ordinary math functions
/// abs, gcd, lcm, reciprocal, exponent, power, trigonometry 等。

#include <cmath>
#include <concepts>
#include <numbers>

namespace ospf::math {

    // -- 绝对值 / Absolute value ----------------------------------------------

    /// 绝对值 / Absolute value
    template<typename T>
    [[nodiscard]] constexpr T abs(T value) noexcept {
        if constexpr (std::is_arithmetic_v<T>) {
            return value < T{0} ? -value : value;
        } else {
            return std::abs(value);
        }
    }

    // -- 倒数 / Reciprocal ----------------------------------------------------

    /// 倒数 / Reciprocal
    template<typename T>
    [[nodiscard]] T reciprocal(T value) {
        return T{1} / value;
    }

    // -- 幂 / Power -----------------------------------------------------------

    /// 平方 / Square
    template<typename T>
    [[nodiscard]] constexpr T square(T value) noexcept {
        return value * value;
    }

    /// 立方 / Cube
    template<typename T>
    [[nodiscard]] constexpr T cube(T value) noexcept {
        return value * value * value;
    }

    // -- 三角函数 / Trigonometric functions ------------------------------------

    /// 正弦（角度制） / Sine (degrees)
    template<typename T>
    [[nodiscard]] T sin_deg(T degrees) {
        return std::sin(degrees * std::numbers::pi_v<T> / T{180});
    }

    /// 余弦（角度制） / Cosine (degrees)
    template<typename T>
    [[nodiscard]] T cos_deg(T degrees) {
        return std::cos(degrees * std::numbers::pi_v<T> / T{180});
    }

    /// 正切（角度制） / Tangent (degrees)
    template<typename T>
    [[nodiscard]] T tan_deg(T degrees) {
        return std::tan(degrees * std::numbers::pi_v<T> / T{180});
    }

    // -- 对数 / Logarithm -----------------------------------------------------

    /// 自然对数 / Natural logarithm
    template<typename T>
    [[nodiscard]] T ln(T value) {
        return std::log(value);
    }

    /// 以 10 为底的对数 / Base-10 logarithm
    template<typename T>
    [[nodiscard]] T log10(T value) {
        return std::log10(value);
    }

    /// 以 2 为底的对数 / Base-2 logarithm
    template<typename T>
    [[nodiscard]] T log2(T value) {
        return std::log2(value);
    }

    // -- 指数 / Exponential ---------------------------------------------------

    /// 指数函数 / Exponential function
    template<typename T>
    [[nodiscard]] T exp(T value) {
        return std::exp(value);
    }

    /// 2 的幂 / Power of 2
    template<typename T>
    [[nodiscard]] T exp2(T value) {
        return std::exp2(value);
    }

    // -- 取整 / Rounding ------------------------------------------------------

    /// 向下取整 / Floor
    template<typename T>
    [[nodiscard]] T floor(T value) {
        return std::floor(value);
    }

    /// 向上取整 / Ceil
    template<typename T>
    [[nodiscard]] T ceil(T value) {
        return std::ceil(value);
    }

    /// 四舍五入 / Round
    template<typename T>
    [[nodiscard]] T round(T value) {
        return std::round(value);
    }

    // -- 质数 / Prime numbers -------------------------------------------------

    /// 检查是否为质数 / Check if prime
    [[nodiscard]] inline bool is_prime(std::int64_t n) {
        if (n < 2) return false;
        if (n < 4) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (std::int64_t i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) return false;
        }
        return true;
    }

}  // namespace ospf::math
