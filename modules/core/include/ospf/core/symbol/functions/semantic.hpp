#pragma once
/// ·ûºÅº¯Êý: semantic
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/semantic.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SemanticFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "semantic"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.empty()) return std::nullopt;
            return inputs[0];
        }
    };

}  // namespace ospf::core
