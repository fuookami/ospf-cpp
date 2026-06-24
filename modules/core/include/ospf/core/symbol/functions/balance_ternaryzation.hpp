#pragma once
/// 符号函数: balance_ternaryzation
/// 1:1 对应 Rust core/symbol/functions/balance_ternaryzation.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>

namespace ospf::core {

    /// balance_ternaryzation 符号函数
    template<typename V = double>
    class BalanceTernaryzationFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "balance_ternaryzation"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if (input < V{0}) return V{-1};
            if (input > V{0}) return V{1};
            return V{0};
        }
    };

}  // namespace ospf::core
