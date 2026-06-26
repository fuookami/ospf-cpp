#pragma once
/// BPP3D domain/layer_assignment/service/limits/demand_constraint /// 1:1 对应 Rust bpp3d/domain/layer_assignment/service/limits/demand_constraint.rs
/// DemandConstraint — 需求约束：为每个需求条目添加上下界约束
/// DemandConstraint: adds lower and upper bound constraints for each demand entry

#include <ospf/framework/bpp3d/domain/layer_assignment/service/load.hpp>
#include <ospf/framework/bpp3d/domain/layer_assignment/service/assignment.hpp>

#include <cstddef>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 需求影子价格键 / Demand shadow price key
    // 对应 Rust DemandShadowPriceKey
    // ============================================================================

    /// 需求影子价格键 / Demand shadow price key
    /// 对应 Rust DemandShadowPriceKey
    struct DemandShadowPriceKey {
        /// 需求模式 / Demand mode
        Bpp3dDemandMode mode = Bpp3dDemandMode::Item;
        /// 需求键 / Demand key
        Bpp3dDemandKey key;
    };

    // ============================================================================
    // 需求约束 / Demand constraint
    // 对应 Rust DemandConstraint<V, U>
    // ============================================================================

    /// 需求约束 / Demand constraint
    /// 对应 Rust DemandConstraint<V, U>
    /// 为每个需求条目添加上下界约束：RMP: demand <= sum(x[layer]*layer_demand[layer]) + overLoad
    struct DemandConstraint {
        /// 名称 / Name
        std::string name = "demand_constraint";

        /// 需求条目 / Demand entries
        std::vector<Bpp3dDemandEntry> demand_entries;

        /// 层数量 / Layer count
        std::size_t layer_count = 0;

        /// 创建 RMP 阶段需求约束 / Create RMP phase demand constraint
        /// 对应 Rust DemandConstraint::imprecise
        [[nodiscard]] static DemandConstraint imprecise(
            std::vector<Bpp3dDemandEntry> entries,
            const ImpreciseAssignment& assignment)
        {
            DemandConstraint c;
            c.demand_entries = std::move(entries);
            c.layer_count = assignment.layer_count();
            return c;
        }

        /// 创建 Final MILP 阶段需求约束 / Create Final MILP phase demand constraint
        /// 对应 Rust DemandConstraint::precise
        [[nodiscard]] static DemandConstraint precise(
            std::vector<Bpp3dDemandEntry> entries,
            const PreciseAssignment& assignment)
        {
            DemandConstraint c;
            c.demand_entries = std::move(entries);
            c.layer_count = assignment.layer_count();
            return c;
        }

        /// 需求影子价格键 / Shadow price key
        /// 对应 Rust DemandConstraint::shadow_price_key
        [[nodiscard]] static DemandShadowPriceKey shadow_price_key(const Bpp3dDemandEntry& entry) {
            return DemandShadowPriceKey{entry.mode, entry.key};
        }

        /// 约束数量 / Constraint count
        [[nodiscard]] std::size_t constraint_count() const noexcept {
            return demand_entries.size();
        }
    };

}  // namespace ospf::framework::bpp3d
