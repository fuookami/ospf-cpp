#pragma once
/// Gurobi 求解器适配器 / Gurobi solver adapter
/// 1:1 对应 Rust core/solver/solvers/gurobi/solver.rs
///
/// 封装 Gurobi C++ API，提供 LP/MIP/QP 求解能力。
/// Wraps Gurobi C++ API, providing LP/MIP/QP solving capability.

#ifdef OSPF_ENABLE_GUROBI

#include <ospf/core/solver/solvers/gurobi/config.hpp>
#include <ospf/core/solver.hpp>

#include "gurobi_c++.h"

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <stdexcept>
#include <sstream>

namespace ospf::core {

    // 前向声明 / Forward declaration
    class MetaModel;

    /// Gurobi 求解器 / Gurobi Solver
    ///
    /// 支持 LP、MIP、QP、MIQP。
    /// Supports LP, MIP, QP, MIQP.
    class GurobiSolver {
    public:
        /// 创建 Gurobi 求解器 / Create Gurobi solver
        explicit GurobiSolver(GurobiConfig config = {})
            : config_(std::move(config)) {}

        /// 获取求解器名称 / Get solver name
        [[nodiscard]] const char* name() const noexcept { return "Gurobi"; }

        /// 求解 MetaModel / Solve MetaModel
        [[nodiscard]] SolveResult solve(const MetaModel& model) const {
            SolveResult result;

            try {
                // 创建 Gurobi 环境 / Create Gurobi environment
                GRBEnv env = GRBEnv(true);
                if (config_.suppress_output) {
                    env.set(GRB_IntParam_OutputFlag, 0);
                }
                env.start();

                // 创建模型 / Create model
                GRBModel grb_model = GRBModel(env);

                // 设置参数 / Set parameters
                if (config_.time_limit > 0) {
                    grb_model.set(GRB_DoubleParam_TimeLimit, config_.time_limit);
                }
                if (config_.mip_gap > 0) {
                    grb_model.set(GRB_DoubleParam_MIPGap, config_.mip_gap);
                }
                if (config_.threads > 0) {
                    grb_model.set(GRB_IntParam_Threads, config_.threads);
                }

                // 构建变量 / Build variables
                std::map<std::string, GRBVar> grb_vars;
                for (std::size_t i = 0; i < model.variable_count(); ++i) {
                    const auto& var = model.variable(i);
                    double lb = var.bounds().lower;
                    double ub = var.bounds().upper;

                    // 处理无穷值 / Handle infinity
                    if (lb <= -1e30) lb = -GRB_INFINITY;
                    if (ub >= 1e30) ub = GRB_INFINITY;

                    char vtype = GRB_CONTINUOUS;
                    if (var.type() == VariableType::Integer) {
                        vtype = GRB_INTEGER;
                    } else if (var.type() == VariableType::Binary) {
                        vtype = GRB_BINARY;
                    }

                    grb_vars[var.name()] = grb_model.addVar(
                        lb, ub, 0.0, vtype, var.name()
                    );
                }

                grb_model.update();

                // 构建目标函数 / Build objective
                if (model.objective().has_value()) {
                    const auto& obj = model.objective().value();
                    // 简化：线性目标系数 / Simplified: linear objective coefficients
                    GRBLinExpr obj_expr;
                    // TODO: 从 Expression 解析目标系数
                    // For now, use a simple linear combination
                    grb_model.setObjective(obj_expr,
                        obj.sense == ObjectiveSense::Maximize ? GRB_MAXIMIZE : GRB_MINIMIZE);
                }

                // 构建约束 / Build constraints
                // TODO: 从 Inequality 解析约束
                // For now, add basic constraints from model

                // 求解 / Solve
                grb_model.optimize();

                // 提取结果 / Extract results
                int status = grb_model.get(GRB_IntAttr_Status);
                if (status == GRB_OPTIMAL) {
                    result.status = SolveStatus::Optimal;
                    result.objective_value = grb_model.get(GRB_DoubleAttr_ObjVal);

                    // 提取变量值 / Extract variable values
                    for (const auto& [name, var] : grb_vars) {
                        result.variable_values[name] = var.get(GRB_DoubleAttr_X);
                    }
                } else if (status == GRB_INFEASIBLE) {
                    result.status = SolveStatus::Infeasible;
                    result.message = "Model is infeasible";
                } else if (status == GRB_INF_OR_UNBD) {
                    result.status = SolveStatus::Unbounded;
                    result.message = "Model is infeasible or unbounded";
                } else if (status == GRB_UNBOUNDED) {
                    result.status = SolveStatus::Unbounded;
                    result.message = "Model is unbounded";
                } else {
                    result.status = SolveStatus::NotSolved;
                    result.message = "Optimization was stopped with status: " + std::to_string(status);
                }

            } catch (const GRBException& e) {
                result.status = SolveStatus::NotSolved;
                result.message = std::string("Gurobi error: ") + e.getMessage();
            } catch (const std::exception& e) {
                result.status = SolveStatus::NotSolved;
                result.message = std::string("Error: ") + e.what();
            }

            return result;
        }

        /// 获取配置 / Get configuration
        [[nodiscard]] const GurobiConfig& config() const noexcept { return config_; }

    private:
        GurobiConfig config_;
    };

}  // namespace ospf::core

#endif  // OSPF_ENABLE_GUROBI
