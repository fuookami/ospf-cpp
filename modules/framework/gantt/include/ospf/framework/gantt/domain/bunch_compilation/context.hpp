#pragma once
/// Gantt domain bunch_compilation/context /// 1:1 对应 Rust gantt/domain/bunch_compilation/context.rs
/// TaskShadowPriceKey + BunchShadowPricePipeline + BasicBunchCompilationContext
/// 依赖 MetaModel（core）

#include <ospf/framework/gantt/domain/bunch.hpp>

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 任务影子价格键 / Task shadow price key
    // 对应 Rust TaskShadowPriceKey
    // ============================================================================

    /// 任务影子价格键 / Task shadow price key
    /// 对应 Rust TaskShadowPriceKey
    struct TaskShadowPriceKey {
        std::size_t task_index = 0;
    };

    // ============================================================================
    // 束影子价格提取 Pipeline / Bunch shadow price extraction pipeline
    // 对应 Rust BunchShadowPricePipeline
    // ============================================================================

    /// 束影子价格提取 Pipeline / Bunch shadow price extraction pipeline
    /// 实现 CGPipeline 以支持影子价格提取
    struct BunchShadowPricePipeline {
        std::size_t n_tasks = 0;

        /// 创建 / Create
        [[nodiscard]] static BunchShadowPricePipeline create(std::size_t n) {
            return BunchShadowPricePipeline{n};
        }

        /// 名称 / Name
        [[nodiscard]] const std::string& name() const {
            static const std::string n = "bunch_shadow_price";
            return n;
        }

        /// 刷新影子价格 / Refresh shadow prices
        /// 对应 Rust Pipeline::refresh
        void refresh(std::unordered_map<std::size_t, double>& shadow_price_map,
                     const std::vector<double>& new_prices) const
        {
            for (std::size_t i = 0; i < new_prices.size() && i < n_tasks; ++i) {
                shadow_price_map[i] = new_prices[i];
            }
        }

        /// 提取器 / Extractor
        /// 返回 lambda：给定 TaskShadowPriceKey → 对应影子价格
        [[nodiscard]] auto extractor(const std::unordered_map<std::size_t, double>& shadow_price_map) const
        {
            return [&shadow_price_map](const TaskShadowPriceKey& key) -> double {
                auto it = shadow_price_map.find(key.task_index);
                return (it != shadow_price_map.end()) ? it->second : 0.0;
            };
        }
    };

    // ============================================================================
    // 束编译上下文 trait / Bunch compilation context trait
    // 对应 Rust IterativeBunchCompilationContext trait
    // ============================================================================

    /// 束编译上下文 / Bunch compilation context
    /// 对应 Rust BasicBunchCompilationContext
    struct BasicBunchCompilationContext {
        std::size_t n_tasks = 0;
        std::vector<std::string> executor_ids;
        bool with_executor_leisure = false;
        std::vector<DomainBunchEntry> bunches;
        std::size_t next_index = 0;

        /// 创建 / Create
        [[nodiscard]] static BasicBunchCompilationContext create(
            std::size_t n_tasks,
            std::vector<std::string> exec_ids,
            bool with_leisure = false)
        {
            return BasicBunchCompilationContext{n_tasks, std::move(exec_ids), with_leisure};
        }

        /// 任务数量 / Task count
        [[nodiscard]] std::size_t task_count() const noexcept { return n_tasks; }

        /// 列数量（当前束数） / Column count (current bunch count)
        [[nodiscard]] std::size_t column_count() const noexcept { return bunches.size(); }

        /// 添加列 / Add columns
        /// 对应 Rust BasicBunchCompilationContext::add_columns
        [[nodiscard]] std::vector<std::size_t> add_columns(
            std::size_t iteration, std::vector<DomainBunchEntry> new_bunches)
        {
            std::vector<std::size_t> added_indices;
            for (auto& bunch : new_bunches) {
                bunch.iteration = iteration;
                bunch.index = next_index;
                bunches.push_back(bunch);
                added_indices.push_back(next_index);
                ++next_index;
            }
            return added_indices;
        }

        /// 移除列 / Remove columns
        /// 对应 Rust BasicBunchCompilationContext::remove_columns
        void remove_columns(const std::vector<std::size_t>& indices) {
            // 标记移除（简化实现：直接移除）
            for (auto idx : indices) {
                auto it = std::find_if(bunches.begin(), bunches.end(),
                    [idx](const DomainBunchEntry& b) { return b.index == idx; });
                if (it != bunches.end()) bunches.erase(it);
            }
        }
    };

}  // namespace ospf::framework::gantt
