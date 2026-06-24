#pragma once
/// ·ûºÅº¯Êý: satisfied_amount_inequality
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/satisfied_amount_inequality.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SatisfiedAmountInequalityFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "satisfied_amount_inequality"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 2) return V{0};
            V count = V{0};
            for (std::size_t i = 0; i + 1 < inputs.size(); i += 2) {
                if (inputs[i] <= inputs[i + 1]) count += V{1};
            }
            return count;
        }
    };

}  // namespace ospf::core
