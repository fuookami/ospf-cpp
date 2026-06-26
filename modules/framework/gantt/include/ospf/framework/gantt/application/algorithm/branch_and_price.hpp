#pragma once
/// Gantt application algorithm/branch_and_price /// 1:1 对应 Rust gantt/application/algorithm/branch_and_price.rs
/// 分支定价框架：BranchNode + BranchDecision + tree search 控制流
/// 探针：验证 C++ 分支定价控制流与 Rust branch_and_price 行为一致

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <ospf/framework/gantt/application/algorithm/policy.hpp>

namespace ospf::framework::gantt {

    // ============================================================================
    // 分支方向 / Branch direction
    // 对应 Rust BranchDirection
    // ============================================================================

    /// 分支方向 / Branch direction
    /// 对应 Rust BranchDirection
    enum class BranchDirection {
        Left,   ///< x == 0 / x <= 0
        Right   ///< x == 1 / x >= 1
    };

    // ============================================================================
    // 分支决策 / Branch decision
    // 对应 Rust BranchDecision
    // ============================================================================

    /// 分支决策 / Branch decision
    /// 对应 Rust BranchDecision
    struct BranchDecision {
        /// 目标变量索引 / Target variable index
        std::size_t target_index = 0;

        /// 分支方向 / Branch direction
        BranchDirection direction = BranchDirection::Left;

        /// 决策类型（zero/one）/ Decision type (zero/one)
        int value = 0;

        /// 创建 zero 决策 / Create zero decision
        /// 对应 Rust BranchDecision::zero
        [[nodiscard]] static BranchDecision zero(std::size_t target, BranchDirection dir) {
            return BranchDecision{target, dir, 0};
        }

        /// 创建 one 决策 / Create one decision
        /// 对应 Rust BranchDecision::one
        [[nodiscard]] static BranchDecision one(std::size_t target, BranchDirection dir) {
            return BranchDecision{target, dir, 1};
        }
    };

    // ============================================================================
    // 分支节点状态 / Branch node status
    // 对应 Rust BranchNodeStatus
    // ============================================================================

    /// 分支节点状态 / Branch node status
    /// 对应 Rust BranchNodeStatus
    enum class BranchNodeStatus {
        Pending,
        Processing,
        Feasible,
        Infeasible,
        Pruned
    };

    // ============================================================================
    // 分支节点 / Branch node
    // 对应 Rust BranchNode
    // ============================================================================

    /// 分支节点 / Branch node
    /// 对应 Rust BranchNode
    struct BranchNode {
        /// 节点 ID / Node ID
        std::size_t id = 0;

        /// 父节点 ID / Parent node ID
        std::optional<std::size_t> parent_id;

        /// 深度 / Depth
        std::size_t depth = 0;

        /// 决策路径 / Decision path
        std::vector<BranchDecision> decisions;

        /// 节点状态 / Node status
        BranchNodeStatus status = BranchNodeStatus::Pending;

        /// 创建根节点 / Create root node
        /// 对应 Rust BranchNode::root
        [[nodiscard]] static BranchNode root() {
            BranchNode node;
            node.id = 0;
            node.parent_id = std::nullopt;
            node.depth = 0;
            node.status = BranchNodeStatus::Pending;
            return node;
        }

        /// 创建子节点 / Create child node
        /// 对应 Rust BranchNode::child
        [[nodiscard]] static BranchNode child(
            std::size_t id,
            const BranchNode& parent,
            BranchDecision decision)
        {
            BranchNode node;
            node.id = id;
            node.parent_id = parent.id;
            node.depth = parent.depth + 1;
            node.decisions = parent.decisions;
            node.decisions.push_back(decision);
            node.status = BranchNodeStatus::Pending;
            return node;
        }
    };

    // ============================================================================
    // 分支搜索结果 / Branch search result
    // 对应 Rust BranchSearchResult<S>
    // ============================================================================

    /// 分支搜索结果 / Branch search result
    /// 对应 Rust BranchSearchResult<S>
    struct BranchSearchResult {
        /// 已处理节点数 / Processed node count
        std::size_t processed_nodes = 0;

        /// 最优解（incumbent）/ Incumbent solution
        std::optional<int> incumbent;

        /// 最优目标值 / Incumbent objective
        std::optional<double> incumbent_objective;

        /// 事件追踪 / Event trace
        std::vector<std::string> trace;
    };

    // ============================================================================
    // 分支搜索配置 / Branch search config
    // 对应 Rust BranchSearchConfig
    // ============================================================================

    /// 分支搜索配置 / Branch search config
    struct BranchSearchConfig {
        std::size_t max_nodes = 100;
        bool best_first = false;
    };

    // ============================================================================
    // 分支定价树搜索 / Branch and price tree search
    /// 对应 Rust branch_and_price tree search 逻辑
    // ============================================================================

    /// 分支定价树搜索 / Branch and price tree search
    /// 对应 Rust BunchBranchAndPriceAlgorithm::run 的树搜索控制流
    /// 探针最小版：BFS 遍历分支树，调用求解回调，更新 incumbent
    class BranchAndPriceTreeSearch {
    public:
        /// 节点求解回调 / Node solve callback
        /// 返回：feasible(目标值) 或 infeasible
        using NodeSolver = std::function<std::optional<double>(const BranchNode&)>;

        /// 节点事件回调 / Node event callback
        using NodeCallback = std::function<void(const BranchNode&, const std::string&)>;

        /// 执行树搜索 / Execute tree search
        /// 对应 Rust tree_search
        [[nodiscard]] static BranchSearchResult search(
            const BranchSearchConfig& config,
            NodeSolver solver,
            NodeCallback node_callback = nullptr)
        {
            BranchSearchResult result;
            std::vector<BranchNode> queue;
            queue.push_back(BranchNode::root());

            std::size_t next_id = 1;

            while (!queue.empty() && result.processed_nodes < config.max_nodes) {
                BranchNode node = std::move(queue.front());
                queue.erase(queue.begin());

                node.status = BranchNodeStatus::Processing;
                if (node_callback) {
                    node_callback(node, "begin:" + std::to_string(node.id));
                }

                // 求解节点 / Solve node
                auto obj = solver(node);

                if (obj.has_value()) {
                    // 可行 / Feasible
                    node.status = BranchNodeStatus::Feasible;
                    if (!result.incumbent_objective.has_value()
                        || *obj < *result.incumbent_objective)
                    {
                        result.incumbent = static_cast<int>(node.id);
                        result.incumbent_objective = *obj;
                        if (node_callback) {
                            node_callback(node, "incumbent:" + std::to_string(node.id));
                        }
                    }

                    // 分支：创建左右子节点 / Branch: create left/right children
                    if (node.depth < 3) {  // 探针限制深度
                        auto left = BranchNode::child(next_id++, node,
                            BranchDecision::zero(0, BranchDirection::Left));
                        auto right = BranchNode::child(next_id++, node,
                            BranchDecision::one(0, BranchDirection::Right));
                        queue.push_back(std::move(left));
                        queue.push_back(std::move(right));
                    }
                } else {
                    // 不可行，剪枝 / Infeasible, prune
                    node.status = BranchNodeStatus::Pruned;
                }

                ++result.processed_nodes;
                if (node_callback) {
                    node_callback(node, "end:" + std::to_string(node.id) + ":" + status_str(node.status));
                }
            }

            return result;
        }

    private:
        [[nodiscard]] static std::string status_str(BranchNodeStatus s) {
            switch (s) {
                case BranchNodeStatus::Feasible: return "Feasible";
                case BranchNodeStatus::Infeasible: return "Infeasible";
                case BranchNodeStatus::Pruned: return "Pruned";
                case BranchNodeStatus::Pending: return "Pending";
                case BranchNodeStatus::Processing: return "Processing";
            }
            return "Unknown";
        }
    };

    // ============================================================================
    // MetaModel 执行诊断 / MetaModel execution diagnostics
    // 对应 Rust Gantt MetaModelExecutionDiagnostics
    // ============================================================================

    /// MetaModel 执行诊断 / MetaModel execution diagnostics
    struct GanttMetaModelDiagnostics {
        std::string model_name;
        std::size_t variable_count = 0;
        std::size_t constraint_count = 0;
        std::size_t task_count = 0;
        std::size_t resource_count = 0;
        std::size_t bunch_count = 0;
    };

    // ============================================================================
    // MetaModel 求解结果 / MetaModel solver result
    // ============================================================================

    /// RMP 执行结果 / RMP execution result
    struct GanttRmpExecution {
        std::optional<double> objective;
        std::unordered_map<std::string, double> shadow_price_summary;
        std::optional<GanttMetaModelDiagnostics> diagnostics;
        std::unordered_map<std::string, std::string> info;
    };

    /// Final MILP 执行结果 / Final MILP execution result
    struct GanttFinalExecution {
        std::vector<BunchEntry> bunches;
        std::optional<double> objective;
        std::optional<GanttMetaModelDiagnostics> diagnostics;
        std::unordered_map<std::string, std::string> info;
    };

    // ============================================================================
    // MetaModel 求解器后端接口 / MetaModel solver backend interface
    // ============================================================================

    /// MetaModel 求解器后端接口 / MetaModel solver backend interface
    class GanttMetaModelSolverBackend {
    public:
        virtual ~GanttMetaModelSolverBackend() = default;
        [[nodiscard]] virtual const std::string& name() const = 0;
        [[nodiscard]] virtual GanttRmpExecution solve_rmp(
            const GanttMetaModelDiagnostics& diagnostics) = 0;
        [[nodiscard]] virtual GanttFinalExecution solve_final(
            const GanttMetaModelDiagnostics& diagnostics) = 0;
    };

    /// Noop MetaModel 求解器后端 / Noop MetaModel solver backend
    class GanttNoopSolverBackend : public GanttMetaModelSolverBackend {
    public:
        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "noop";
            return n;
        }
        [[nodiscard]] GanttRmpExecution solve_rmp(const GanttMetaModelDiagnostics&) override {
            return {};
        }
        [[nodiscard]] GanttFinalExecution solve_final(const GanttMetaModelDiagnostics&) override {
            return {};
        }
    };

    // ============================================================================
    // RMP 执行器接口 / RMP executor interface
    // ============================================================================

    /// RMP 执行器接口 / RMP executor interface
    class GanttRmpExecutor {
    public:
        virtual ~GanttRmpExecutor() = default;
        [[nodiscard]] virtual GanttRmpExecution execute(
            const std::vector<BunchEntry>& bunches,
            const std::vector<std::string>& task_ids) = 0;
    };

    /// Final MILP 执行器接口 / Final MILP executor interface
    class GanttFinalExecutor {
    public:
        virtual ~GanttFinalExecutor() = default;
        [[nodiscard]] virtual GanttFinalExecution execute(
            const std::vector<BunchEntry>& bunches,
            const std::vector<std::string>& task_ids) = 0;
    };

}  // namespace ospf::framework::gantt
