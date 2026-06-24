#pragma once
/// ·ûºÅº¯Êý: masking
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/masking.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class MaskingFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "masking"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V mask, V value) const override {
            return (mask != V{0}) ? value : V{0};
        }
    };

}  // namespace ospf::core
