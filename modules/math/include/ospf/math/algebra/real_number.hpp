#pragma once

/// RealNumber concept — 实数类型概念 / Real number type concept
///
/// 合成概念，Rust 无直接对应。覆盖 double/float；BigDecimal/BigRational 后续 phase 接入。
/// Synthesized concept with no direct Rust equivalent. Covers double/float;
/// BigDecimal/BigRational to be integrated in later phases.
///
/// from_decimal 不进基础 concept（double/float 无该成员），改为单独 customization point。
/// from_decimal is not part of the base concept (double/float lack it);
/// provided as a separate customization point via free function overloads.

#include <concepts>
#include <optional>
#include <string>
#include <type_traits>

namespace ospf::math {

    // ============================================================================
    // RealNumber concept
    // ============================================================================

    /// RealNumber：实数类型概念 / Real number type concept
    /// 要求：可拷贝 + 全序 + 算术运算符 + 零值/单位值可构造。
    /// Requires: copyable + totally ordered + arithmetic operators + zero/one constructible.
    template<typename V>
    concept RealNumber = std::copyable<V>
        && std::totally_ordered<V>
        && requires(V a, V b) {
            { a + b } -> std::convertible_to<V>;
            { a - b } -> std::convertible_to<V>;
            { a * b } -> std::convertible_to<V>;
            { a / b } -> std::convertible_to<V>;
            { -a } -> std::convertible_to<V>;
            { V(0) } -> std::same_as<V>;   // 零值可构造 / Zero constructible
            { V(1) } -> std::same_as<V>;   // 单位值可构造 / Unit constructible
        };

    // ============================================================================
    // from_decimal — customization point (自由函数重载 / free function overloads)
    // ============================================================================

    /// 从十进制字符串构造数值 / Construct numeric value from decimal string
    /// double/float 默认实现；BigDecimal/BigRational 后续 phase 提供特化。
    /// Default implementation for double/float; BigDecimal/BigRational specializations in later phases.
    template<typename V>
    [[nodiscard]] auto from_decimal(std::string_view str) -> std::optional<V>;

    /// double 特化 / double specialization
    template<>
    [[nodiscard]] inline auto from_decimal<double>(std::string_view str) -> std::optional<double> {
        try {
            std::string s(str);
            size_t pos = 0;
            double val = std::stod(s, &pos);
            if (pos == s.size()) {
                return val;
            }
            return std::nullopt;
        } catch (...) {
            return std::nullopt;
        }
    }

    /// float 特化 / float specialization
    template<>
    [[nodiscard]] inline auto from_decimal<float>(std::string_view str) -> std::optional<float> {
        try {
            std::string s(str);
            size_t pos = 0;
            float val = std::stof(s, &pos);
            if (pos == s.size()) {
                return val;
            }
            return std::nullopt;
        } catch (...) {
            return std::nullopt;
        }
    }

    // ============================================================================
    // 静态断言：验证 double/float 满足 RealNumber / Static assertions
    // ============================================================================

    static_assert(RealNumber<double>, "double must satisfy RealNumber");
    static_assert(RealNumber<float>,  "float must satisfy RealNumber");

}  // namespace ospf::math
