#pragma once
/// 符号函数: cos
/// 1:1 对应 Rust core/symbol/functions/cos.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <cmath>
#include <optional>

namespace ospf::core {

    template<typename V = double>
    class CosFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "cos"; }
        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) { return std::cos(input); }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
