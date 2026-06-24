#pragma once
/// ·ûºÅº¯Êý: min_max
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/min_max.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class MinMaxFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "min_max"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 3) return std::nullopt;
            return std::clamp(inputs[0], inputs[1], inputs[2]);
        }
    };

}  // namespace ospf::core
