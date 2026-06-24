#pragma once
/// ·ûºÅº¯Êý: sigmoid
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/sigmoid.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class SigmoidFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "sigmoid"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) { return V{1} / (V{1} + std::exp(-input)); }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
