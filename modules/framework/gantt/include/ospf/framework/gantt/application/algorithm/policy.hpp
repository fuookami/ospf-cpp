#pragma once
/// Gantt application algorithm/policy /// 1:1 对应 Rust gantt/application/algorithm/policy.rs
/// BunchCGPolicy 接口：束级列生成策略

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 束条目 / Bunch entry
    // 对应 Rust BunchEntry
    // ============================================================================

    /// 束条目 / Bunch entry
    /// 对应 Rust BunchEntry
    struct BunchEntry {
        /// 束 ID / Bunch ID
        std::string id;
        /// 包含的任务 ID / Contained task IDs
        std::vector<std::string> task_ids;
        /// reduced cost / Reduced cost
        double reduced_cost = 0.0;
    };

    // ============================================================================
    // 束级列生成策略 / Bunch column generation policy
    // 对应 Rust BunchCGPolicy trait
    // ============================================================================

    /// 束级列生成策略 / Bunch column generation policy
    /// 对应 Rust BunchCGPolicy trait
    class BunchCGPolicy {
    public:
        virtual ~BunchCGPolicy() = default;

        /// 策略名称 / Policy name
        [[nodiscard]] virtual const std::string& name() const = 0;

        /// 构建影子价格映射 / Build shadow price map
        /// 对应 Rust BunchCGPolicy::build_shadow_price_map
        [[nodiscard]] virtual std::unordered_map<std::string, double> build_shadow_price_map() const = 0;

        /// 计算 reduced cost / Calculate reduced cost
        /// 对应 Rust BunchCGPolicy::reduced_cost
        [[nodiscard]] virtual double reduced_cost(
            const std::unordered_map<std::string, double>& shadow_prices,
            const BunchEntry& bunch) const = 0;

        /// 生成新束 / Generate new bunches
        /// 对应 Rust BunchCGPolicy::generate_bunches
        [[nodiscard]] virtual std::vector<BunchEntry> generate_bunches(
            std::size_t iteration,
            const std::vector<std::string>& executor_ids,
            const std::unordered_map<std::string, double>& shadow_prices) const = 0;
    };

}  // namespace ospf::framework::gantt
