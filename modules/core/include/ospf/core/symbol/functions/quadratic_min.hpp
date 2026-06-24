#pragma once
/// ·ûºÅº¯Êý: quadratic_min
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/quadratic_min.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class QuadraticMinFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "quadratic_min"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            return std::min(a, b);
        }
    };

}  // namespace ospf::core
