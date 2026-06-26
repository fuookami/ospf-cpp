#pragma once
/// Gantt domain task/cost /// 1:1 对应 Rust gantt/domain/task/cost.rs
/// CostItem + Cost + MutableCost

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 成本项 / Cost item
    // 对应 Rust CostItem<V>
    // ============================================================================

    /// 成本项 / Cost item
    /// 对应 Rust CostItem<V>
    struct CostItem {
        /// 标签 / Tag
        std::string tag;
        /// 成本数量（可选，None 表示无效） / Cost quantity (optional, None = invalid)
        std::optional<double> cost_quantity;
        /// 消息 / Message
        std::optional<std::string> message;

        /// 创建带数量的成本项 / Create cost item with quantity
        /// 对应 Rust CostItem::with_quantity
        [[nodiscard]] static CostItem with_quantity(std::string tag, double quantity) {
            return CostItem{std::move(tag), quantity, std::nullopt};
        }

        /// 是否有效 / Valid check
        /// 对应 Rust CostItem::valid
        [[nodiscard]] bool valid() const noexcept {
            return cost_quantity.has_value();
        }
    };

    // ============================================================================
    // 成本 / Cost
    // 对应 Rust Cost<V>
    // ============================================================================

    /// 成本 / Cost
    /// 对应 Rust Cost<V>
    struct Cost {
        /// 成本项列表 / Cost items
        std::vector<CostItem> items;
        /// 成本总和（所有有效项之和，无效项存在则为 None） / Cost sum (sum of valid items, None if any invalid)
        std::optional<double> cost_sum;

        /// 创建空成本 / Create empty cost
        /// 对应 Rust Cost::empty
        [[nodiscard]] static Cost empty() {
            return Cost{{}, 0.0};
        }

        /// 创建成本 / Create cost
        /// 对应 Rust Cost::new
        [[nodiscard]] static Cost create(std::vector<CostItem> new_items) {
            Cost c;
            c.items = std::move(new_items);
            // 计算 cost_sum：所有有效项之和，任一无效则 None
            double sum = 0.0;
            for (const auto& item : c.items) {
                if (!item.valid()) {
                    c.cost_sum = std::nullopt;
                    return c;
                }
                sum += *item.cost_quantity;
            }
            c.cost_sum = sum;
            return c;
        }

        /// 是否有效 / Valid check
        [[nodiscard]] bool valid() const noexcept {
            return cost_sum.has_value();
        }

        /// 计算求解器成本（加固定成本）/ Compute solver cost (+ fixed cost)
        /// 对应 Rust Cost::solver_cost
        [[nodiscard]] double solver_cost(double fixed_cost = 0.0) const noexcept {
            return cost_sum.value_or(0.0) + fixed_cost;
        }

        /// 合并两个成本 / Merge two costs
        /// 对应 Rust Cost::merge
        [[nodiscard]] Cost merge(const Cost& other) const {
            Cost merged;
            merged.items = items;
            merged.items.insert(merged.items.end(), other.items.begin(), other.items.end());
            // 重新计算 cost_sum
            double sum = 0.0;
            for (const auto& item : merged.items) {
                if (!item.valid()) {
                    merged.cost_sum = std::nullopt;
                    return merged;
                }
                sum += *item.cost_quantity;
            }
            merged.cost_sum = sum;
            return merged;
        }
    };

    // ============================================================================
    // 可变成本 / Mutable cost
    // 对应 Rust MutableCost<V>
    // ============================================================================

    /// 可变成本 / Mutable cost
    /// 对应 Rust MutableCost<V>
    struct MutableCost {
        /// 成本项列表 / Cost items
        std::vector<CostItem> items;
        /// 成本总和 / Cost sum
        std::optional<double> cost_sum;

        /// 创建空可变成本 / Create empty mutable cost
        /// 对应 Rust MutableCost::new (cost_sum 初始化为 0.0，非 None)
        [[nodiscard]] static MutableCost create() {
            MutableCost mc;
            mc.cost_sum = 0.0;
            return mc;
        }

        /// 添加成本项 / Add cost item
        /// 对应 Rust MutableCost::add_item
        void add_item(CostItem item) {
            items.push_back(std::move(item));
            // 重新计算 cost_sum
            if (!items.back().valid()) {
                cost_sum = std::nullopt;
                return;
            }
            if (!cost_sum.has_value()) {
                // 先前有无效项，保持 None
                return;
            }
            cost_sum = *cost_sum + *items.back().cost_quantity;
        }
    };

}  // namespace ospf::framework::gantt
