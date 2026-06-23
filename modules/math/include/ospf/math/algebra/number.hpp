#pragma once

/// 数值类型与 RealNumber concept / Numeric types and RealNumber concept
/// 覆盖 double/float/boost::multiprecision。

#include <ospf/math/algebra/concept.hpp>
#include <cmath>
#include <concepts>
#include <limits>
#include <numbers>

namespace ospf::math::algebra {

    // -- RealNumber concept ---------------------------------------------------

    /// RealNumber：实数类型概念 / RealNumber: real number type concept
    /// 要求满足浮点类型 + 基本数学运算 + 全序
    template<typename T>
    concept RealNumber = std::floating_point<T>
        && TotallyOrdered<T>
        && requires(const T& a, const T& b) {
            { a + b } -> std::convertible_to<T>;
            { a - b } -> std::convertible_to<T>;
            { a * b } -> std::convertible_to<T>;
            { a / b } -> std::convertible_to<T>;
            { -a } -> std::convertible_to<T>;
            { std::abs(a) } -> std::convertible_to<T>;
            { std::sqrt(a) } -> std::convertible_to<T>;
            { std::pow(a, b) } -> std::convertible_to<T>;
            { std::log(a) } -> std::convertible_to<T>;
            { std::exp(a) } -> std::convertible_to<T>;
            { std::sin(a) } -> std::convertible_to<T>;
            { std::cos(a) } -> std::convertible_to<T>;
        };

    // -- 数值特征 / Numeric traits --------------------------------------------

    /// 数值特征 / Numeric traits
    template<RealNumber T>
    struct NumericTraits {
        [[nodiscard]] static constexpr T zero()     noexcept { return T{0}; }
        [[nodiscard]] static constexpr T one()      noexcept { return T{1}; }
        [[nodiscard]] static constexpr T epsilon()  noexcept { return std::numeric_limits<T>::epsilon(); }
        [[nodiscard]] static constexpr T min_value() noexcept { return std::numeric_limits<T>::lowest(); }
        [[nodiscard]] static constexpr T max_value() noexcept { return std::numeric_limits<T>::max(); }
        [[nodiscard]] static constexpr T positive_infinity() noexcept { return std::numeric_limits<T>::infinity(); }
        [[nodiscard]] static constexpr T negative_infinity() noexcept { return -std::numeric_limits<T>::infinity(); }
        [[nodiscard]] static constexpr T pi()       noexcept { return std::numbers::pi_v<T>; }
        [[nodiscard]] static constexpr T e()        noexcept { return std::numbers::e_v<T>; }

        [[nodiscard]] static bool is_nan(T v)  noexcept { return std::isnan(v); }
        [[nodiscard]] static bool is_inf(T v)  noexcept { return std::isinf(v); }
        [[nodiscard]] static bool is_finite(T v) noexcept { return std::isfinite(v); }
    };

    // -- 模运算 / Modular arithmetic ------------------------------------------

    /// 模加法 / Modular addition
    template<std::integral T>
    [[nodiscard]] constexpr T mod_add(T a, T b, T mod) noexcept {
        return (a + b) % mod;
    }

    /// 模乘法 / Modular multiplication
    template<std::integral T>
    [[nodiscard]] constexpr T mod_mul(T a, T b, T mod) noexcept {
        return (a * b) % mod;
    }

    /// 最大公约数 / Greatest common divisor
    template<std::integral T>
    [[nodiscard]] constexpr T gcd(T a, T b) noexcept {
        while (b != 0) {
            T t = b;
            b = a % b;
            a = t;
        }
        return a;
    }

    /// 最小公倍数 / Least common multiple
    template<std::integral T>
    [[nodiscard]] constexpr T lcm(T a, T b) noexcept {
        return a / gcd(a, b) * b;
    }

}  // namespace ospf::math::algebra
