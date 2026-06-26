#pragma once
/// Gantt domain capacity_scheduling/model /// 1:1 对应 Rust gantt/domain/capacity_scheduling/model.rs
/// BasicProductionAction + ProductionActionTrait + CapacityCompilation

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 生产行为 trait / Production action trait
    // 对应 Rust ProductionActionTrait
    // ============================================================================

    /// 生产行为 trait / Production action trait
    /// 对应 Rust ProductionActionTrait
    class ProductionActionTrait {
    public:
        virtual ~ProductionActionTrait() = default;
        [[nodiscard]] virtual const std::string& id() const = 0;
        [[nodiscard]] virtual const std::string& name() const = 0;
        [[nodiscard]] virtual const std::string& executor_id() const = 0;
        [[nodiscard]] virtual double unit_capacity() const = 0;
        [[nodiscard]] virtual double unit_cost() const = 0;
        [[nodiscard]] virtual bool discrete() const = 0;
    };

    // ============================================================================
    // 基础生产行为 / Basic production action
    // 对应 Rust BasicProductionAction
    // ============================================================================

    /// 基础生产行为 / Basic production action
    /// 对应 Rust BasicProductionAction
    struct BasicProductionAction : public ProductionActionTrait {
        std::string id_;
        std::string name_;
        std::string executor_id_;
        double unit_capacity_ = 0.0;
        double unit_cost_ = 0.0;

        BasicProductionAction() = default;
        BasicProductionAction(std::string id, std::string name, std::string exec,
                              double cap, double cost)
            : id_(std::move(id)), name_(std::move(name)), executor_id_(std::move(exec))
            , unit_capacity_(cap), unit_cost_(cost) {}

        [[nodiscard]] const std::string& id() const override { return id_; }
        [[nodiscard]] const std::string& name() const override { return name_; }
        [[nodiscard]] const std::string& executor_id() const override { return executor_id_; }
        [[nodiscard]] double unit_capacity() const override { return unit_capacity_; }
        [[nodiscard]] double unit_cost() const override { return unit_cost_; }
        [[nodiscard]] bool discrete() const override { return false; }
    };

    // ============================================================================
    // 容量编译 / Capacity compilation
    // 对应 Rust CapacityCompilation
    // ============================================================================

    /// 容量编译 / Capacity compilation
    /// 对应 Rust CapacityCompilation
    struct CapacityCompilation {
        std::vector<BasicProductionAction> actions;
        std::vector<std::string> executor_ids;
        std::size_t slot_count = 0;

        [[nodiscard]] static CapacityCompilation create(
            std::vector<BasicProductionAction> acts,
            std::vector<std::string> exec_ids,
            std::size_t slots)
        {
            return CapacityCompilation{std::move(acts), std::move(exec_ids), slots};
        }

        [[nodiscard]] bool empty() const noexcept { return actions.empty(); }
    };

}  // namespace ospf::framework::gantt
