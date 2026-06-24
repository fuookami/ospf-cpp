#pragma once
/// ·ûºÅº¯Êý: same_as
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/same_as.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SameAsFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "same_as"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            return (a == b) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
