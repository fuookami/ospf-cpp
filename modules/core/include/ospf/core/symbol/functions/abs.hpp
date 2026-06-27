#pragma once
/// 符号函数: abs / Symbol function: abs
/// 1:1 对应 Rust core/symbol/functions/abs.rs
/// result = |x|
/// 建模语义: result >= x, result >= -x

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

        /// 需要辅助变量（结果变量） / Needs auxiliary variable (result variable)
        [[nodiscard]] bool has_auxiliary_variables() const noexcept override { return true; }

        /// 1 个辅助变量 / 1 auxiliary variable
        [[nodiscard]] std::size_t auxiliary_variable_count() const noexcept override { return 1; }

        /// 生成约束: result >= x, result >= -x
        [[nodiscard]] std::vector<LinearConstraint<V>> generate_mechanism_constraints(
            const std::vector<std::size_t>& input_indices,
            std::size_t auxiliary_start_index) const override
        {
            std::vector<LinearConstraint<V>> constraints;
            if (input_indices.empty()) return constraints;

            auto x_idx = input_indices[0];
            auto result_idx = auxiliary_start_index;

            // result >= x  →  result - x >= 0
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[x_idx] = V{-1};
                c.rhs = V{0};
                c.sense = LinearConstraint<V>::Sense::GreaterEqual;
                constraints.push_back(c);
            }

            // result >= -x  →  result + x >= 0
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[x_idx] = V{1};
                c.rhs = V{0};
                c.sense = LinearConstraint<V>::Sense::GreaterEqual;
                constraints.push_back(c);
            }

            return constraints;
        }
    };

}  // namespace ospf::core
