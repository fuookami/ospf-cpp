#pragma once
/// ·ûºÅº¯Êý: quadratic_function
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/quadratic_function.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class QuadraticFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "quadratic_function"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 4) return std::nullopt;
            V x = inputs[0], a = inputs[1], b = inputs[2], c = inputs[3];
            return a * x * x + b * x + c;
        }
    };

}  // namespace ospf::core
