#pragma once
/// ·ûºÅº¯Êý: one_of
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/one_of.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class OneOfFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "one_of"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            V sum = V{0};
            for (auto v : inputs) sum += v;
            return (sum == V{1}) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
