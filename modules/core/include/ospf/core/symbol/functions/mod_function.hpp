#pragma once
/// ·ûºÅº¯Êý: mod_function
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/mod_function.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class ModFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "mod_function"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            if (b == V{0}) return std::nullopt;
            if constexpr (std::is_integral_v<V>) { return a % b; }
            else { return std::fmod(a, b); }
        }
    };

}  // namespace ospf::core
