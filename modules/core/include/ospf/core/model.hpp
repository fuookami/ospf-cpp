#pragma once

/// 模型系统 / Model system
/// MetaModel 是模型的核心，管理变量、约束、目标。

#include <ospf/core/variable.hpp>
#include <ospf/math/symbol/expression.hpp>
#include <ospf/math/symbol/inequality.hpp>
#include <functional>
#include <string>
#include <vector>
#include <memory>

namespace ospf::core {

    /// 模型构建阶段 / Model building stage
    enum class ModelStage : std::uint8_t {
        Registration,  ///< 注册阶段 / Registration
        Building,      ///< 构建阶段 / Building
        Solving,       ///< 求解阶段 / Solving
        Solved,        ///< 已求解 / Solved
    };

    /// 约束 / Constraint
    struct Constraint {
        std::string name;
        ospf::math::symbol::Inequality inequality;

        Constraint(std::string n, ospf::math::symbol::Inequality ineq)
            : name(std::move(n)), inequality(std::move(ineq)) {}
    };

    /// 目标 / Objective
    enum class ObjectiveSense : std::uint8_t {
        Minimize,
        Maximize,
    };

    struct Objective {
        std::string name;
        ObjectiveSense sense;
        ospf::math::symbol::Expression expression;

        Objective(std::string n, ObjectiveSense s, ospf::math::symbol::Expression expr)
            : name(std::move(n)), sense(s), expression(std::move(expr)) {}
    };

    /// 元模型 / MetaModel: core model managing variables, constraints, objectives
    class MetaModel {
    public:
        MetaModel(std::string name = "unnamed")
            : name_(std::move(name)), stage_(ModelStage::Registration) {}

        // -- 变量管理 / Variable management ------------------------------------

        /// 注册变量 / Register variable
        Variable& add_variable(Variable var) {
            variables_.push_back(std::move(var));
            return variables_.back();
        }

        /// 获取变量 / Get variable by index
        [[nodiscard]] Variable& variable(std::size_t index) { return variables_[index]; }
        [[nodiscard]] const Variable& variable(std::size_t index) const { return variables_[index]; }

        /// 变量数量 / Variable count
        [[nodiscard]] std::size_t variable_count() const noexcept { return variables_.size(); }

        /// 获取所有变量 / Get all variables
        [[nodiscard]] const std::vector<Variable>& variables() const noexcept { return variables_; }

        // -- 约束管理 / Constraint management ----------------------------------

        /// 添加约束 / Add constraint
        void add_constraint(std::string name, ospf::math::symbol::Inequality ineq) {
            constraints_.emplace_back(std::move(name), std::move(ineq));
        }

        /// 约束数量 / Constraint count
        [[nodiscard]] std::size_t constraint_count() const noexcept { return constraints_.size(); }

        /// 获取所有约束 / Get all constraints
        [[nodiscard]] const std::vector<Constraint>& constraints() const noexcept { return constraints_; }

        // -- 目标管理 / Objective management -----------------------------------

        /// 设置目标 / Set objective
        void set_objective(std::string name, ObjectiveSense sense, ospf::math::symbol::Expression expr) {
            objective_ = Objective(std::move(name), sense, std::move(expr));
        }

        /// 获取目标 / Get objective
        [[nodiscard]] const std::optional<Objective>& objective() const noexcept { return objective_; }

        // -- 阶段管理 / Stage management ---------------------------------------

        [[nodiscard]] ModelStage stage() const noexcept { return stage_; }
        void set_stage(ModelStage stage) { stage_ = stage; }

        // -- 查询 / Queries ----------------------------------------------------

        [[nodiscard]] const std::string& name() const noexcept { return name_; }

        /// 检查模型是否有效 / Check if model is valid
        [[nodiscard]] bool is_valid() const noexcept {
            return !variables_.empty() && objective_.has_value();
        }

    private:
        std::string name_;
        ModelStage stage_;
        std::vector<Variable> variables_;
        std::vector<Constraint> constraints_;
        std::optional<Objective> objective_;
    };

    /// 线性元模型 / Abstract linear meta model
    /// 约束和目标都是线性表达式的模型
    class AbstractLinearMetaModel : public MetaModel {
    public:
        using MetaModel::MetaModel;

        /// 添加线性约束 / Add linear constraint
        void add_linear_constraint(std::string name, ospf::math::symbol::Inequality ineq) {
            add_constraint(std::move(name), std::move(ineq));
        }

        /// 设置线性目标 / Set linear objective
        void set_linear_objective(std::string name, ObjectiveSense sense,
                                  ospf::math::symbol::Expression expr) {
            set_objective(std::move(name), sense, std::move(expr));
        }
    };

}  // namespace ospf::core
