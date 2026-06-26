#pragma once
/// Gantt domain bunch_generation/label /// 1:1 对应 Rust gantt/domain/bunch_generation/label.rs
/// Node + TaskNode + Label — 列生成定价标签（部分路径）

#include <ospf/framework/gantt/domain/task/cost.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 任务节点 / Task node
    // 对应 Rust TaskNode
    // ============================================================================

    /// 任务节点 / Task node
    struct TaskNode {
        std::size_t task_index = 0;
        std::size_t node_index = 0;
        double time = 0.0;
    };

    // ============================================================================
    // 节点 / Node
    // 对应 Rust Node（Root / End / Task）
    // ============================================================================

    /// 节点类型 / Node type
    enum class NodeType { Root, End, Task };

    /// 节点 / Node
    struct Node {
        NodeType type = NodeType::Root;
        std::optional<TaskNode> task_node;

        [[nodiscard]] static Node root() { return Node{NodeType::Root, std::nullopt}; }
        [[nodiscard]] static Node end() { return Node{NodeType::End, std::nullopt}; }
        [[nodiscard]] static Node task(TaskNode tn) { return Node{NodeType::Task, std::move(tn)}; }

        [[nodiscard]] bool operator==(const Node& o) const noexcept {
            if (type != o.type) return false;
            if (type == NodeType::Task && task_node && o.task_node)
                return task_node->task_index == o.task_node->task_index;
            return true;
        }
    };

    // ============================================================================
    // 标签 / Label
    // 对应 Rust Label
    // ============================================================================

    /// 标签 / Label
    /// 表示定价图中的部分路径，包含累积成本、影子价格和路径追踪
    struct Label {
        Cost cost;
        double shadow_price = 0.0;
        std::shared_ptr<Label> prev_label;
        Node node;
        std::optional<std::size_t> task_index;

        /// 创建根标签 / Create root label
        [[nodiscard]] static Label root() {
            Label l;
            l.cost = Cost::empty();
            l.shadow_price = 0.0;
            l.node = Node::root();
            return l;
        }

        /// 创建任务标签 / Create task label
        [[nodiscard]] static Label task(
            const Label& prev, Node nd, std::size_t ti, Cost c, double sp)
        {
            Label l;
            l.cost = c;
            l.shadow_price = sp;
            l.prev_label = std::make_shared<Label>(prev);
            l.node = std::move(nd);
            l.task_index = ti;
            return l;
        }

        /// 创建终止标签 / Create end label
        [[nodiscard]] static Label end(const Label& prev) {
            Label l;
            l.cost = prev.cost;
            l.shadow_price = prev.shadow_price;
            l.prev_label = std::make_shared<Label>(prev);
            l.node = Node::end();
            return l;
        }

        /// 是否更优束 / Is better bunch
        [[nodiscard]] bool is_better_bunch() const noexcept {
            return false; // 简化：默认不优于现有束
        }

        /// 缩减成本 / Reduced cost
        [[nodiscard]] double reduced_cost() const noexcept {
            return cost.cost_sum.value_or(0.0) - shadow_price;
        }

        /// 是否访问过节点 / Whether visited node
        [[nodiscard]] bool visited(const Node& target) const {
            if (node == target) return true;
            if (prev_label) return prev_label->visited(target);
            return false;
        }

        /// 生成束任务 / Generate bunch tasks
        [[nodiscard]] std::optional<std::vector<std::size_t>> generate_bunch_tasks() const {
            std::vector<std::size_t> tasks;
            const Label* cur = this;
            while (cur) {
                if (cur->task_index.has_value()) {
                    tasks.push_back(*cur->task_index);
                }
                cur = cur->prev_label.get();
            }
            if (tasks.empty()) return std::nullopt;
            std::reverse(tasks.begin(), tasks.end());
            return tasks;
        }
    };

    // ============================================================================
    // 标签支配关系 / Label dominance
    // ============================================================================

    /// 判断 label_a 是否支配 label_b / Whether label_a dominates label_b
    [[nodiscard]] inline bool label_dominates(const Label& a, const Label& b) {
        // a 支配 b：a 的成本 <= b 且 a 的影子价格 >= b
        double a_cost = a.cost.cost_sum.value_or(0.0);
        double b_cost = b.cost.cost_sum.value_or(0.0);
        if (a_cost > b_cost) return false;
        if (a.shadow_price < b.shadow_price) return false;
        return (a_cost < b_cost) || (a.shadow_price > b.shadow_price);
    }

}  // namespace ospf::framework::gantt
