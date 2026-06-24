#pragma once
/// ·ûºÅº¯Êý: slack_range
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/slack_range.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SlackRangeFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "slack_range"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            V value = inputs[0], lower = inputs[1], upper = inputs[2];
            if (value < lower) return lower - value;
            if (value > upper) return value - upper;
            return V{0};
        }
    };

}  // namespace ospf::core
