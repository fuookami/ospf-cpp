#pragma once
/// 符号函数: big_m
/// 1:1 对应 Rust core/symbol/functions/big_m.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>

namespace ospf::core {

    struct BigMPolicy {
        double fallback = 1000000.0;
        double min = 1.0;
        [[nodiscard]] double resolve(std::optional<double> inferred) const {
            return std::max(inferred.value_or(fallback), min);
        }
    };

    template<typename V = double>
    class BigMFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "big_m"; }
        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.empty()) return V{0};
            return inputs[0];
        }
    };

}  // namespace ospf::core
