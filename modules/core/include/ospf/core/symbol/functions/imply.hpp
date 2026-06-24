#pragma once
/// ·ûºÅº¯Êý: imply
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/imply.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class ImplyFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "imply"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            bool a_true = (a != V{0});
            bool b_true = (b != V{0});
            return (!a_true || b_true) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
