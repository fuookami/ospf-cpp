#pragma once
/// ·ûºÅº¯Êý: max
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/max.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class MaxFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "max"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            return std::max(a, b);
        }
    };

}  // namespace ospf::core
