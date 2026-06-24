#pragma once

/// 代数定律验证 / Algebraic law verification
/// 1:1 对应 Rust algebra/law/mod.rs

#include <concepts>
#include <type_traits>

namespace ospf::math::algebra {

    /// 验证交换律 / Verify commutativity
    template<typename T>
        requires requires(const T& a, const T& b) { { a + b } -> std::convertible_to<T>; }
    [[nodiscard]] constexpr bool is_commutative(const T& a, const T& b) noexcept {
        return a + b == b + a;
    }

    /// 验证结合律 / Verify associativity
    template<typename T>
        requires requires(const T& a, const T& b, const T& c) {
            { a + b } -> std::convertible_to<T>;
            { (a + b) + c } -> std::convertible_to<T>;
            { a + (b + c) } -> std::convertible_to<T>;
        }
    [[nodiscard]] constexpr bool is_associative(const T& a, const T& b, const T& c) noexcept {
        return (a + b) + c == a + (b + c);
    }

    /// 验证分配律 / Verify distributivity
    template<typename T>
        requires requires(const T& a, const T& b, const T& c) {
            { a * (b + c) } -> std::convertible_to<T>;
            { a * b + a * c } -> std::convertible_to<T>;
        }
    [[nodiscard]] constexpr bool is_distributive(const T& a, const T& b, const T& c) noexcept {
        return a * (b + c) == a * b + a * c;
    }

}  // namespace ospf::math::algebra
