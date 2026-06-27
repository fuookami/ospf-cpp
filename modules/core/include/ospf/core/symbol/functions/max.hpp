#pragma once
/// 符号函数: max / Symbol function: max
/// 1:1 对应 Rust core/symbol/functions/max.rs
/// result = max(a, b)
/// 建模语义: result >= a, result >= b, Big-M 选择约束

#include <ospf/core/symbol/function_symbol.hpp>
#include <algorithm>
#include <optional>

namespace ospf::core {

    /// max 符号函数 / max symbol function
    /// result = max(a, b)
    template<typename V = double>
    class MaxFunction : public BinaryFunctionSymbol<V> {
    public:
        [[nodiscard]] const char* name() const noexcept override { return "max"; }

        [[nodiscard]] std::optional<V> evaluate_binary(V a, V b) const override {
            return std::max(a, b);
        }

        /// 需要辅助变量（结果 + 二元选择） / Needs auxiliary variables
        [[nodiscard]] bool has_auxiliary_variables() const noexcept override { return true; }

        /// 2 个辅助变量: result, z
        [[nodiscard]] std::size_t auxiliary_variable_count() const noexcept override { return 2; }

        /// 生成 Big-M 约束 / Generate Big-M constraints
        [[nodiscard]] std::vector<LinearConstraint<V>> generate_mechanism_constraints(
            const std::vector<std::size_t>& input_indices,
            std::size_t auxiliary_start_index) const override
        {
            std::vector<LinearConstraint<V>> constraints;
            if (input_indices.size() < 2) return constraints;

            auto a_idx = input_indices[0];
            auto b_idx = input_indices[1];
            auto result_idx = auxiliary_start_index;
            auto z_idx = auxiliary_start_index + 1;
            V big_m = V{1000000};

            // result >= a
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[a_idx] = V{-1};
                c.rhs = V{0};
                c.sense = LinearConstraint<V>::Sense::GreaterEqual;
                constraints.push_back(c);
            }

            // result >= b
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[b_idx] = V{-1};
                c.rhs = V{0};
                c.sense = LinearConstraint<V>::Sense::GreaterEqual;
                constraints.push_back(c);
            }

            // result <= a + M*(1-z)
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[a_idx] = V{-1};
                c.coefficients[z_idx] = big_m;
                c.rhs = big_m;
                c.sense = LinearConstraint<V>::Sense::LessEqual;
                constraints.push_back(c);
            }

            // result <= b + M*z
            {
                LinearConstraint<V> c;
                c.coefficients[result_idx] = V{1};
                c.coefficients[b_idx] = V{-1};
                c.coefficients[z_idx] = -big_m;
                c.rhs = V{0};
                c.sense = LinearConstraint<V>::Sense::LessEqual;
                constraints.push_back(c);
            }

            return constraints;
        }
    };

}  // namespace ospf::core
