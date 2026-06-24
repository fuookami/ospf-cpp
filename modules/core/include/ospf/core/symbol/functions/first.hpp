#pragma once
/// 符号函数: first
/// 1:1 对应 Rust core/symbol/functions/first.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>

namespace ospf::core {

    template<typename V = double>
    class FirstFunction : public VariadicFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "first"; }
        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.empty()) return std::nullopt;
            return inputs[0];
        }
    };

}  // namespace ospf::core
