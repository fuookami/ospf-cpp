#pragma once
/// ·ûºÅº¯Êý: univariate_linear_piecewise
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/univariate_linear_piecewise.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class UnivariateLinearPiecewiseFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "univariate_linear_piecewise"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            V x = inputs[0], slope = inputs[1], intercept = inputs[2];
            return slope * x + intercept;
        }
    };

}  // namespace ospf::core
