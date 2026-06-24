#pragma once
/// ·ûºÅº¯Êý: inequality
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/inequality.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class InequalityFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "inequality"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            return (a <= b) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
