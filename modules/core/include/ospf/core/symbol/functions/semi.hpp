#pragma once
/// ·ûºÅº¯Êý: semi
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/semi.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SemiFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "semi"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 4) return std::nullopt;
            V indicator = inputs[0], value = inputs[1], lower = inputs[2], upper = inputs[3];
            if (indicator == V{0}) return V{0};
            if (value < lower) return lower;
            if (value > upper) return upper;
            return value;
        }
    };

}  // namespace ospf::core
