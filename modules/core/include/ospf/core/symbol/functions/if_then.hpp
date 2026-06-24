#pragma once
/// ·ûºÅº¯Êý: if_then
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/if_then.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class IfThenFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "if_then"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V condition, V value) const override {
            return (condition != V{0}) ? value : V{0};
        }
    };

}  // namespace ospf::core
