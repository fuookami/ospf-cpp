#pragma once
/// ·ûºÅº¯Êý: product
/// 1:1 ¶ÔÓ¦ Rust core/symbol/functions/product.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>
#include <algorithm>
#include <cmath>

namespace ospf::core {

    template<typename V = double>
    class ProductFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "product"; }

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.empty()) return V{1};
            V result = V{1};
            for (auto v : inputs) result *= v;
            return result;
        }
    };

}  // namespace ospf::core
