#pragma once
/// Gantt application algorithm/bunch_column_generation /// 1:1 对应 Rust gantt/application/algorithm/bunch_column_generation.rs
/// 束级分支定价算法：BunchBranchAndPriceAlgorithm::run 主循环
/// 编排：bunch generation → column generation → branch-and-price tree search

#include <ospf/framework/gantt/application/algorithm/branch_and_price.hpp>
#include <ospf/framework/gantt/application/algorithm/policy.hpp>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 排程配置 / Scheduling configuration
    // 对应 Rust GanttSchedulingConfiguration
    // ============================================================================

    /// 排程配置 / Scheduling configuration
    /// 对应 Rust GanttSchedulingConfiguration
    struct GanttSchedulingConfig {
        /// 最大迭代次数 / Maximum iterations
        int max_iterations = 50;
        /// 时间限制（秒）/ Time limit (seconds)
        double time_limit = 300.0;
        /// 最大未改进迭代次数 / Maximum non-improving iterations
        int max_not_better_iterations = 5;
        /// 分支搜索配置 / Branch search config
        BranchSearchConfig branch_config;
    };

    // ============================================================================
    // 排程结果 / Scheduling result
    // 对应 Rust BunchSolution
    // ============================================================================

    /// 排程结果 / Scheduling result
    /// 对应 Rust BunchSolution
    struct GanttSchedulingResult {
        /// 最优目标值 / Best objective
        std::optional<double> best_objective;
        /// 活跃束 / Active bunches
        std::vector<BunchEntry> active_bunches;
        /// 迭代次数 / Iteration count
        int iteration = 0;
        /// 终止原因 / Termination reason
        std::string termination_reason;
        /// 分支搜索结果 / Branch search result
        BranchSearchResult branch_result;
    };

    // ============================================================================
    // 束级分支定价算法 / Bunch branch and price algorithm
    // 对应 Rust BunchBranchAndPriceAlgorithm<C, S, P>
    // 主循环：bunch generation → column generation → branch-and-price tree search
    // ============================================================================

    /// 束级分支定价算法 / Bunch branch and price algorithm
    /// 对应 Rust BunchBranchAndPriceAlgorithm<C, S, P>
    class BunchBranchAndPriceAlgorithm {
    public:
        /// 节点求解回调 / Node solve callback
        using NodeSolver = std::function<std::optional<double>(const BranchNode&)>;

        /// 构造 / Construct
        BunchBranchAndPriceAlgorithm(
            GanttSchedulingConfig config,
            std::shared_ptr<BunchCGPolicy> policy,
            NodeSolver solver = nullptr)
            : config_(std::move(config))
            , policy_(std::move(policy))
            , solver_(std::move(solver))
        {}

        /// 执行排程 / Run scheduling
        /// 对应 Rust BunchBranchAndPriceAlgorithm::run
        /// 主循环：初始化 → 列生成迭代 → 分支定价树搜索 → 返回最优解
        [[nodiscard]] GanttSchedulingResult run() {
            GanttSchedulingResult result;
            result.iteration = 0;

            // 1. 初始化影子价格 / Initialize shadow prices
            auto shadow_prices = policy_->build_shadow_price_map();

            // 2. 主循环：列生成迭代 / Main loop: column generation iteration
            int not_better_count = 0;
            std::optional<double> prev_obj;

            while (result.iteration < config_.max_iterations) {
                // 生成新束 / Generate new bunches
                std::vector<std::string> executor_ids;
                auto new_bunches = policy_->generate_bunches(
                    result.iteration, executor_ids, shadow_prices);

                if (new_bunches.empty()) {
                    result.termination_reason = "PricingConverged";
                    break;
                }

                // 计算 reduced cost 并筛选 / Calculate reduced cost and filter
                std::vector<BunchEntry> improving;
                for (auto& bunch : new_bunches) {
                    bunch.reduced_cost = policy_->reduced_cost(shadow_prices, bunch);
                    if (bunch.reduced_cost < -config_.branch_config.max_nodes * 1e-6) {
                        improving.push_back(std::move(bunch));
                    }
                }

                if (improving.empty()) {
                    ++not_better_count;
                    if (not_better_count >= config_.max_not_better_iterations) {
                        result.termination_reason = "ImprovementStalled";
                        break;
                    }
                } else {
                    not_better_count = 0;
                    for (auto& b : improving) {
                        active_bunches_.push_back(std::move(b));
                    }
                }

                // 更新影子价格 / Update shadow prices
                shadow_prices = policy_->build_shadow_price_map();
                ++result.iteration;
            }

            if (result.termination_reason.empty()) {
                result.termination_reason = "IterationLimitReached";
            }

            // 3. 分支定价树搜索 / Branch and price tree search
            if (solver_) {
                auto branch_result = BranchAndPriceTreeSearch::search(
                    config_.branch_config, solver_);
                result.branch_result = std::move(branch_result);
                result.best_objective = result.branch_result.incumbent_objective;
            }

            result.active_bunches = active_bunches_;
            return result;
        }

        /// 获取活跃束 / Get active bunches
        [[nodiscard]] const std::vector<BunchEntry>& active_bunches() const {
            return active_bunches_;
        }

        /// 获取配置 / Get config
        [[nodiscard]] const GanttSchedulingConfig& config() const {
            return config_;
        }

    private:
        GanttSchedulingConfig config_;
        std::shared_ptr<BunchCGPolicy> policy_;
        NodeSolver solver_;
        std::vector<BunchEntry> active_bunches_;
    };

}  // namespace ospf::framework::gantt
