#pragma once

/// 求解器系统 / Solver system
/// SolverTrait、SolveResult、stub solver。

#include <ospf/core/model.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::core {

    /// 求解状态 / Solve status
    enum class SolveStatus : std::uint8_t {
        Optimal,       ///< 最优解 / Optimal solution found
        Feasible,      ///< 可行解（非最优） / Feasible solution (not optimal)
        Infeasible,    ///< 不可行 / Infeasible
        Unbounded,     ///< 无界 / Unbounded
        Error,         ///< 错误 / Error
        NotSolved,     ///< 未求解 / Not solved
    };

    /// 求解结果 / Solve result
    struct SolveResult {
        SolveStatus status = SolveStatus::NotSolved;
        std::optional<double> objective_value;
        std::unordered_map<std::string, double> variable_values;
        std::string message;

        /// MIP gap (相对间隙)
        std::optional<double> mip_gap;
        /// 迭代次数
        std::optional<std::size_t> iteration_count;
        /// 节点数 (MIP)
        std::optional<std::size_t> node_count;

        [[nodiscard]] bool is_optimal() const noexcept {
            return status == SolveStatus::Optimal;
        }

        [[nodiscard]] bool is_feasible() const noexcept {
            return status == SolveStatus::Optimal || status == SolveStatus::Feasible;
        }
    };

    /// 求解器 trait（抽象基类） / Solver trait (abstract base class)
    class SolverTrait {
    public:
        virtual ~SolverTrait() = default;

        /// 求解模型 / Solve model
        [[nodiscard]] virtual SolveResult solve(const MetaModel& model) = 0;

        /// 求解器名称 / Solver name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 是否可用 / Is available
        [[nodiscard]] virtual bool is_available() const = 0;
    };

    // ===========================================================================
    // Stub solver — 返回可预测解，供测试使用
    // Stub solver — returns predictable solutions for testing
    // ===========================================================================

    /// Stub 求解器 / Stub solver
    class StubSolver : public SolverTrait {
    public:
        [[nodiscard]] SolveResult solve(const MetaModel& model) override {
            SolveResult result;
            result.status = SolveStatus::Optimal;
            result.objective_value = 0.0;
            result.message = "Stub solver: optimal";

            // 为每个变量赋默认值（下界）
            for (std::size_t i = 0; i < model.variable_count(); ++i) {
                const auto& var = model.variable(i);
                result.variable_values[var.name()] = var.bounds().lower;
            }

            // 计算目标值
            if (model.objective().has_value()) {
                // 简单计算：所有变量取下界时的目标值
                double obj_val = 0.0;
                for (const auto& [name, val] : result.variable_values) {
                    obj_val += val;  // 简化：假设目标是变量之和
                }
                result.objective_value = obj_val;
            }

            return result;
        }

        [[nodiscard]] const std::string& name() const override {
            static const std::string name = "StubSolver";
            return name;
        }

        [[nodiscard]] bool is_available() const override { return true; }
    };

    // ===========================================================================
    // Flatten — 将 MetaModel 展平为线性表达式
    // ===========================================================================

    /// 展平结果 / Flatten result
    struct FlattenResult {
        std::vector<std::pair<std::string, double>> coefficients;
        double constant = 0.0;
    };

    /// 展平表达式（简化版） / Flatten expression (simplified)
    [[nodiscard]] inline FlattenResult flatten_expression(
        const ospf::math::symbol::Expression& expr,
        const std::unordered_map<std::string, double>& variable_values)
    {
        FlattenResult result;
        result.constant = expr.evaluate(
            [&]() {
                std::map<ospf::math::symbol::Variable, double> values;
                for (const auto& [name, val] : variable_values) {
                    values[ospf::math::symbol::Variable(name, 0)] = val;
                }
                return values;
            }()
        );
        return result;
    }

    // ===========================================================================
    // IIS（不可行子集） — 简化版
    // IIS (Irreducible Infeasible Subset) — simplified
    // ===========================================================================

    /// IIS 结果 / IIS result
    struct IisResult {
        std::vector<std::string> constraint_names;
        std::vector<std::string> variable_names;
        bool found = false;
    };

    /// 计算 IIS（stub 实现） / Compute IIS (stub implementation)
    [[nodiscard]] inline IisResult compute_iis(const MetaModel& model) {
        IisResult result;
        // stub：返回所有约束
        for (const auto& c : model.constraints()) {
            result.constraint_names.push_back(c.name);
        }
        result.found = !result.constraint_names.empty();
        return result;
    }

    // ===========================================================================
    // 输出格式化 / Output formatting
    // ===========================================================================

    /// 格式化求解结果 / Format solve result
    [[nodiscard]] inline std::string format_solve_result(const SolveResult& result) {
        std::string output;
        output += "Status: ";
        switch (result.status) {
            case SolveStatus::Optimal:    output += "Optimal"; break;
            case SolveStatus::Feasible:   output += "Feasible"; break;
            case SolveStatus::Infeasible: output += "Infeasible"; break;
            case SolveStatus::Unbounded:  output += "Unbounded"; break;
            case SolveStatus::Error:      output += "Error"; break;
            case SolveStatus::NotSolved:  output += "Not Solved"; break;
        }
        output += "\n";

        if (result.objective_value.has_value()) {
            output += "Objective: " + std::to_string(*result.objective_value) + "\n";
        }

        if (!result.variable_values.empty()) {
            output += "Variables:\n";
            for (const auto& [name, val] : result.variable_values) {
                output += "  " + name + " = " + std::to_string(val) + "\n";
            }
        }

        if (!result.message.empty()) {
            output += "Message: " + result.message + "\n";
        }

        return output;
    }

}  // namespace ospf::core
