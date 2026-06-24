#pragma once
/// 符号函数: and / Symbol function: and
/// 1:1 对应 Rust core/symbol/functions/and.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>

namespace ospf::core {

    /// and 符号函数 / and symbol function
    /// result = a AND b (逻辑与，用于 0/1 变量)
    template<typename V = double>
    class AndFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "and"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            bool a_true = (a != V{0});
            bool b_true = (b != V{0});
            return (a_true && b_true) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
