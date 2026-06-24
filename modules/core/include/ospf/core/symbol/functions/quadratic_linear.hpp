#pragma once
/// ·ûºÅº¯Êý: quadratic_linear
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/quadratic_linear.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class QuadraticLinearFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "quadratic_linear"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            V x = inputs[0], a = inputs[1], b = inputs[2];
            return a * x + b;
        }
    };

}  // namespace ospf::core
