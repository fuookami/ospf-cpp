#pragma once
/// ·ûºÅº¯Êý: if_in
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/if_in.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class IfInFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "if_in"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            V value = inputs[0], lower = inputs[1], upper = inputs[2];
            return (value >= lower && value <= upper) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
