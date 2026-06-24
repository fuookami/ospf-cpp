#pragma once
/// ·ûºÅº¯Êý: quadratic_masking_range
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/quadratic_masking_range.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class QuadraticMaskingRangeFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "quadratic_masking_range"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 4) return std::nullopt;
            V value = inputs[0], mask = inputs[1], lower = inputs[2], upper = inputs[3];
            if (mask == V{0}) return V{0};
            return (value >= lower && value <= upper) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
