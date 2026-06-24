#pragma once
/// 符号函数: binaryzation
/// 1:1 对应 Rust core/symbol/functions/binaryzation.rs

#include <ospf/core/symbol/function_symbol.hpp>
#include <optional>

namespace ospf::core {

    template<typename V = double>
    class BinaryzationFunction : public UnaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "binaryzation"; }
        [[nodiscard]] std::optional<V> evaluate_single(V input) const override {
            return (input != V{0}) ? V{1} : V{0};
        }
    };

}  // namespace ospf::core
