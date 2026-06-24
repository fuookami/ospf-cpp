#pragma once
/// ·ûºÅº¯Êý: sin
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/sin.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SinFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "sin"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) { return std::sin(input); }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
