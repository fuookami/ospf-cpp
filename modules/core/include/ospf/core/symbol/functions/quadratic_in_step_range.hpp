#pragma once
/// ·ûºÅº¯Êý: quadratic_in_step_range
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/quadratic_in_step_range.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class QuadraticInStepRangeFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "quadratic_in_step_range"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            V value = inputs[0], lower = inputs[1], step = inputs[2];
            V normalized = value - lower;
            if (step == V{0}) return V{0};
            V quotient = normalized / step;
            return (quotient >= V{0} && quotient < V{1}) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
