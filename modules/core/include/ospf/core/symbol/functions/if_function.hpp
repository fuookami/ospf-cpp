#pragma once
/// ·ûºÅº¯Êý: if_function
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/if_function.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class IfFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "if_function"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            return (inputs[0] != V{0}) ? inputs[1] : inputs[2];
        }
    };

}  // namespace ospf::core
