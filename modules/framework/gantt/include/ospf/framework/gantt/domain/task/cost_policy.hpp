#pragma once
/// Gantt domain task/cost_policy /// 1:1 对应 Rust gantt/domain/task/cost_policy.rs
/// CostBreakdown + DefaultBunchCostPolicy + FunctionalBunchCostPolicy

#include <ospf/framework/gantt/domain/bunch.hpp>

#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 成本分解 / Cost breakdown
    // 对应 Rust CostBreakdown
    // ============================================================================

    /// 成本分解 / Cost breakdown
    /// 对应 Rust CostBreakdown
    struct CostBreakdown {
        double task_cost = 0.0;
        double executor_cost = 0.0;
        double connection_cost = 0.0;
        double capacity_cost = 0.0;
        double penalty_cost = 0.0;

        /// 创建空成本分解 / Create empty cost breakdown
        [[nodiscard]] static CostBreakdown zero() {
            return CostBreakdown{};
        }

        /// 总成本 / Total cost
        [[nodiscard]] double total() const noexcept {
            return task_cost + executor_cost + connection_cost + capacity_cost + penalty_cost;
        }
    };

    // ============================================================================
    // 默认束成本策略 / Default bunch cost policy
    // 对应 Rust DefaultBunchCostPolicy
    // ============================================================================

    /// 默认束成本策略 / Default bunch cost policy
    /// reduced_cost = bunch.cost - sum(shadow_prices[task_indices])
    struct DefaultBunchCostPolicy {
        /// 计算 reduced cost / Calculate reduced cost
        [[nodiscard]] double reduced_cost(
            const DomainBunchEntry& bunch,
            const std::unordered_map<std::size_t, double>& shadow_prices) const
        {
            double sp_sum = 0.0;
            for (auto idx : bunch.task_indices) {
                auto it = shadow_prices.find(idx);
                if (it != shadow_prices.end()) sp_sum += it->second;
            }
            return bunch.cost - sp_sum;
        }
    };

    // ============================================================================
    // 功能束成本策略 / Functional bunch cost policy
    // 对应 Rust FunctionalBunchCostPolicy
    // ============================================================================

    /// 功能束成本策略 / Functional bunch cost policy
    /// 允许注入业务惩罚逻辑
    struct FunctionalBunchCostPolicy {
        using CostBreakdownFn = std::function<CostBreakdown(
            const DomainBunchEntry&,
            const std::unordered_map<std::size_t, double>&)>;

        CostBreakdownFn cost_breakdown_fn;

        [[nodiscard]] CostBreakdown cost_breakdown(
            const DomainBunchEntry& bunch,
            const std::unordered_map<std::size_t, double>& shadow_prices) const
        {
            return cost_breakdown_fn(bunch, shadow_prices);
        }

        [[nodiscard]] double reduced_cost(
            const DomainBunchEntry& bunch,
            const std::unordered_map<std::size_t, double>& shadow_prices) const
        {
            auto breakdown = cost_breakdown(bunch, shadow_prices);
            return breakdown.total();
        }
    };

}  // namespace ospf::framework::gantt
