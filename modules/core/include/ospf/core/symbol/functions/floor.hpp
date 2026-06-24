#pragma once
/// 符号函数: floor
/// 1:1 对应 Rust core/symbol/functions/floor.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <cmath>
#include <optional>

namespace ospf::core {

    template<typename V = double>
    class FloorFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "floor"; }
        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            if constexpr (std::is_arithmetic_v<V>) { return std::floor(input); }
            return std::nullopt;
        }
    };

}  // namespace ospf::core
