#pragma once
/// FunctionSymbol 基类 / FunctionSymbol base class
/// 1:1 对应 Rust core/symbol/function_symbol.rs
/// 支持数值求值 + 建模语义（辅助变量 + mechanism constraints）

#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ospf::core {

    // ============================================================================
    // LinearConstraint — 线性约束 / Linear constraint
    // ============================================================================

    /// 线性约束 / Linear constraint
    template<typename V = double>
    struct LinearConstraint {
        /// 系数: variable_index -> coefficient
        std::unordered_map<std::size_t, V> coefficients;
        /// 右侧常量
        V rhs = V{};
        /// 约束方向
        enum class Sense : std::uint8_t { LessEqual, Equal, GreaterEqual };
        Sense sense = Sense::LessEqual;
    };

    // ============================================================================
    // FunctionSymbol — 函数符号基类 / Function symbol base class
    // ============================================================================

    /// 函数符号基类 / Function symbol base class
    /// 优化建模用符号函数，支持求值和约束生成。
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

        // -- 建模语义 / Modeling semantics ------------------------------------

        /// 是否需要辅助变量 / Whether this function needs auxiliary variables
        [[nodiscard]] virtual bool has_auxiliary_variables() const noexcept { return false; }

        /// 辅助变量数量 / Number of auxiliary variables needed
        [[nodiscard]] virtual std::size_t auxiliary_variable_count() const noexcept { return 0; }

        /// 生成 mechanism 约束 / Generate mechanism constraints
        /// 输入变量索引、辅助变量索引起始值 → 约束列表
        [[nodiscard]] virtual std::vector<LinearConstraint<V>> generate_mechanism_constraints(
            const std::vector<std::size_t>& input_indices,
            std::size_t auxiliary_start_index) const
        {
            (void)input_indices;
            (void)auxiliary_start_index;
            return {};
        }
    };

    // ============================================================================
    // UnaryFunctionSymbol — 一元函数 / Unary function
    // ============================================================================

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

    // ============================================================================
    // BinaryFunctionSymbol — 二元函数 / Binary function
    // ============================================================================

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

    // ============================================================================
    // VariadicFunctionSymbol — 多元函数 / Variadic function
    // ============================================================================

    /// 多元函数符号 / Variadic function symbol
    template<typename V = double>
    class VariadicFunctionSymbol : public FunctionSymbol<V> {
    public:
        [[nodiscard]] std::size_t input_count() const noexcept override { return 0; } // 0 = variadic
    };

}  // namespace ospf::core
