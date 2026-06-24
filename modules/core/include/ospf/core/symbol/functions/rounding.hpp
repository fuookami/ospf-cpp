#pragma once
/// ·ûºÅº¯Êý: rounding
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/rounding.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class RoundingFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "rounding"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) { return std::round(input); }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
