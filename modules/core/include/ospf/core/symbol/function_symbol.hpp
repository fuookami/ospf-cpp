#pragma once
/// FunctionSymbol 基类 / FunctionSymbol base class
/// 1:1 对应 Rust core/symbol/function_symbol.rs

#include <string>
#include <optional>
#include <vector>
#include <memory>

namespace ospf::core {

    /// 函数符号基类 / Function symbol base class
    /// 优化建模用符号函数，支持求值和约束生成。
    /// Symbol function for optimization modeling, supports evaluation and constraint generation.
    template<typename V = double>
    class FunctionSymbol {
    public:
        virtual ~FunctionSymbol() = default;

        /// 获取函数名 / Get function name
        [[nodiscard]] virtual const char* name() const noexcept = 0;

        /// 从输入值计算函数值 / Calculate function value from input values
        [[nodiscard]] virtual std::optional<V> evaluate(const std::vector<V>& inputs) const = 0;

        /// 获取输入变量数量 / Get number of input variables
        [[nodiscard]] virtual std::size_t input_count() const noexcept = 0;
    };

    /// 一元函数符号 / Unary function symbol
    template<typename V = double>
    class UnaryFunctionSymbol : public FunctionSymbol<V> {
    public:
        [[nodiscard]] std::size_t input_count() const noexcept override { return 1; }

        /// 一元函数求值 / Unary function evaluation
        [[nodiscard]] virtual std::optional<V> evaluate_single(V input) const = 0;

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 1) return std::nullopt;
            return evaluate_single(inputs[0]);
        }
    };

    /// 二元函数符号 / Binary function symbol
    template<typename V = double>
    class BinaryFunctionSymbol : public FunctionSymbol<V> {
    public:
        [[nodiscard]] std::size_t input_count() const noexcept override { return 2; }

        /// 二元函数求值 / Binary function evaluation
        [[nodiscard]] virtual std::optional<V> evaluate_binary(V a, V b) const = 0;

        [[nodiscard]] std::optional<V> evaluate(const std::vector<V>& inputs) const override {
            if (inputs.size() < 2) return std::nullopt;
            return evaluate_binary(inputs[0], inputs[1]);
        }
    };

    /// 多元函数符号 / Variadic function symbol
    template<typename V = double>
    class VariadicFunctionSymbol : public FunctionSymbol<V> {
    public:
        [[nodiscard]] std::size_t input_count() const noexcept override { return 0; } // 0 = variadic
    };

}  // namespace ospf::core
