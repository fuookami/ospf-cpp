#pragma once
/// Gantt domain bunch /// 1:1 对应 Rust gantt/domain/bunch.rs
/// DomainBunchEntry + BunchAggregation + BunchSolution + BunchSolutionSummary
/// 注：BunchEntry 在 policy.hpp 中定义（列生成框架），DomainBunchEntry 是领域层的束条目

#include <cstddef>
#include <string>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 领域层束条目 / Domain bunch entry
    // 对应 Rust bunch_compilation::model::BunchEntry
    // 注：与 policy.hpp 的 BunchEntry（列生成框架）是不同类型
    // ============================================================================

    /// 领域层束条目 / Domain bunch entry
    /// 对应 Rust bunch_compilation::model::BunchEntry
    struct DomainBunchEntry {
        /// 索引 / Index
        std::size_t index = 0;
        /// 执行器 ID / Executor ID
        std::string executor_id;
        /// 任务索引列表 / Task indices
        std::vector<std::size_t> task_indices;
        /// 成本 / Cost
        double cost = 0.0;
        /// 迭代 / Iteration
        std::size_t iteration = 0;

        /// 任务数量 / Task count
        [[nodiscard]] std::size_t task_count() const noexcept { return task_indices.size(); }

        /// 克隆 / Clone
        [[nodiscard]] DomainBunchEntry clone() const { return *this; }
    };

    // ============================================================================
    // 束聚合 / Bunch aggregation
    // 对应 Rust BunchAggregation
    // ============================================================================

    /// 束聚合 / Bunch aggregation
    /// 对应 Rust BunchAggregation
    struct BunchAggregation {
        /// 已添加的束列表 / Added bunches
        std::vector<DomainBunchEntry> bunches;
        /// 下一个分配的索引 / Next allocated index
        std::size_t next_index = 0;

        /// 创建空聚合 / Create empty aggregation
        /// 对应 Rust BunchAggregation::new
        [[nodiscard]] static BunchAggregation create() {
            return BunchAggregation{};
        }

        /// 添加束 / Add bunches
        /// 对应 Rust BunchAggregation::add_bunches
        /// 返回添加的束的索引列表
        [[nodiscard]] std::vector<std::size_t> add_bunches(std::size_t iter, std::vector<DomainBunchEntry> new_bunches) {
            std::vector<std::size_t> added_indices;
            for (auto& bunch : new_bunches) {
                bunch.iteration = iter;
                bunch.index = next_index;
                bunches.push_back(bunch);
                added_indices.push_back(next_index);
                ++next_index;
            }
            return added_indices;
        }
    };

    // ============================================================================
    // 束解决方案汇总 / Bunch solution summary
    // 对应 Rust BunchSolutionSummary
    // ============================================================================

    /// 束解决方案汇总 / Bunch solution summary
    struct BunchSolutionSummary {
        std::size_t bunch_count = 0;
        std::size_t canceled_task_count = 0;
        std::size_t total_task_count = 0;
    };

    // ============================================================================
    // 束解决方案 / Bunch solution
    // 对应 Rust BunchSolution
    // ============================================================================

    /// 束解决方案 / Bunch solution
    /// 对应 Rust BunchSolution
    struct BunchSolution {
        /// 选中的束 / Selected bunches
        std::vector<std::size_t> selected_bunches;
        /// 取消的任务 / Canceled tasks
        std::vector<std::size_t> canceled_tasks;

        /// 汇总 / Summary
        /// 对应 Rust BunchSolution::summary
        [[nodiscard]] BunchSolutionSummary summary() const {
            return BunchSolutionSummary{
                selected_bunches.size(),
                canceled_tasks.size(),
                selected_bunches.size() + canceled_tasks.size()
            };
        }
    };

}  // namespace ospf::framework::gantt
