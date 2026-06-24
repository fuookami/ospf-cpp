#pragma once
/// 符号函数: bivariate_linear_piecewise
/// 1:1 对应 Rust core/symbol/functions/bivariate_linear_piecewise.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>

namespace ospf::core {

    template<typename V = double>
    class BivariateLinearPiecewiseFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "bivariate_linear_piecewise"; }
        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 2) return std::nullopt;
            return inputs[0] + inputs[1];
        }
    };

}  // namespace ospf::core
