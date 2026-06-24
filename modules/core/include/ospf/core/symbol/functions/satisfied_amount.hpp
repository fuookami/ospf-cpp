#pragma once
/// ·ûºÅº¯Êý: satisfied_amount
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/satisfied_amount.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SatisfiedAmountFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "satisfied_amount"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            V count = V{0};
            for (auto v : inputs) { if (v != V{0}) count += V{1}; }
            return count;
        }
    };

}  // namespace ospf::core
