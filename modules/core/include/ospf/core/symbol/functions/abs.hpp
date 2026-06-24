#pragma once
/// 符号函数: abs / Symbol function: abs
/// 1:1 对应 Rust core/symbol/functions/abs.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <cmath>
#include <optional>

namespace ospf::core {

    /// abs 符号函数 / abs symbol function
    /// result = |x|
    template<typename V = double>
    class AbsFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "abs"; }

        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) {
                return std::abs(input);
            }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
