#pragma once
/// Gantt domain task/task_step_graph /// 1:1 对应 Rust gantt/domain/task/task_step_graph.rs
/// StepRelation + TaskStep + TaskStepGraph（builder + DAG 验证）

#include <cstddef>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    // ============================================================================
    // 步骤关系 / Step relation
    // 对应 Rust StepRelation
    // ============================================================================

    /// 步骤关系 / Step relation
    enum class StepRelation { And, Or };

    // ============================================================================
    // 任务步骤 / Task step
    // 对应 Rust TaskStepTrait
    // ============================================================================

    /// 任务步骤 / Task step
    struct TaskStep {
        std::string id;
        std::string name;
    };

    // ============================================================================
    // 任务步骤图 / Task step graph
    // 对应 Rust TaskStepGraph
    // ============================================================================

    /// 任务步骤图 / Task step graph
    struct TaskStepGraph {
        std::string id;
        std::string name;
        std::vector<TaskStep> steps;
        std::unordered_map<std::string, std::size_t> step_index;
        std::vector<std::vector<std::string>> forward_edges;
        std::vector<std::vector<std::string>> backward_edges;
        std::vector<std::string> start_steps;
        StepRelation start_relation = StepRelation::And;

        /// 获取步骤 / Get step
        [[nodiscard]] const TaskStep* step(const std::string& sid) const {
            auto it = step_index.find(sid);
            if (it != step_index.end()) return &steps[it->second];
            return nullptr;
        }

        /// 后继 / Successors
        [[nodiscard]] std::vector<std::string> successors(const std::string& sid) const {
            auto it = step_index.find(sid);
            if (it != step_index.end()) return forward_edges[it->second];
            return {};
        }

        /// 前驱 / Predecessors
        [[nodiscard]] std::vector<std::string> predecessors(const std::string& sid) const {
            auto it = step_index.find(sid);
            if (it != step_index.end()) return backward_edges[it->second];
            return {};
        }

        // ========================================================================
        // Builder
        // ========================================================================

        struct Builder {
            std::string graph_id, graph_name;
            std::vector<TaskStep> steps_;
            std::unordered_map<std::string, std::size_t> idx_;
            std::vector<std::vector<std::string>> fwd_, bwd_;
            std::vector<std::string> start_steps_;
            StepRelation start_rel_ = StepRelation::And;
            std::string err_;

            Builder(std::string gid, std::string gname) : graph_id(std::move(gid)), graph_name(std::move(gname)) {}

            Builder& add_step(TaskStep s) {
                if (idx_.count(s.id)) { err_ = "duplicated"; return *this; }
                idx_[s.id] = steps_.size();
                steps_.push_back(std::move(s));
                fwd_.emplace_back();
                bwd_.emplace_back();
                return *this;
            }
            Builder& add_steps(std::vector<TaskStep> sv) { for (auto& s : sv) add_step(std::move(s)); return *this; }
            Builder& start_steps(std::vector<std::string> ids, StepRelation r) { start_steps_ = std::move(ids); start_rel_ = r; return *this; }
            Builder& forward(const std::string& from, std::vector<std::string> tos, StepRelation) {
                auto it = idx_.find(from); if (it == idx_.end()) { err_ = "unknown"; return *this; }
                for (auto& t : tos) { if (!idx_.count(t)) { err_ = "unknown"; return *this; } fwd_[it->second].push_back(t); }
                return *this;
            }
            Builder& backward(std::vector<std::string> froms, const std::string& to, StepRelation) {
                auto it = idx_.find(to); if (it == idx_.end()) { err_ = "unknown"; return *this; }
                for (auto& f : froms) { if (!idx_.count(f)) { err_ = "unknown"; return *this; } bwd_[it->second].push_back(f); }
                return *this;
            }

            /// 构建（含 DAG 验证） / Build (with DAG validation)
            [[nodiscard]] std::optional<TaskStepGraph> build() {
                if (!err_.empty()) return std::nullopt;
                for (auto& s : start_steps_) { if (!idx_.count(s)) { err_ = "unknown task step id"; return std::nullopt; } }
                // 前向/后向一致性
                for (std::size_t i = 0; i < fwd_.size(); ++i) {
                    for (auto& to : fwd_[i]) {
                        auto ti = idx_.at(to);
                        bool ok = false;
                        for (auto& b : bwd_[ti]) { if (b == steps_[i].id) { ok = true; break; } }
                        if (!ok) { err_ = "missing backward vector"; return std::nullopt; }
                    }
                }
                // DAG 拓扑排序
                std::vector<int> in_d(steps_.size(), 0);
                for (std::size_t i = 0; i < fwd_.size(); ++i) for (auto& t : fwd_[i]) in_d[idx_.at(t)]++;
                std::queue<std::size_t> q;
                for (std::size_t i = 0; i < steps_.size(); ++i) if (in_d[i] == 0) q.push(i);
                int vis = 0;
                while (!q.empty()) {
                    auto u = q.front(); q.pop(); vis++;
                    for (auto& t : fwd_[u]) { if (--in_d[idx_.at(t)] == 0) q.push(idx_.at(t)); }
                }
                if (vis != static_cast<int>(steps_.size())) { err_ = "DAG cycle detected"; return std::nullopt; }

                TaskStepGraph g;
                g.id = graph_id; g.name = graph_name;
                g.steps = steps_; g.step_index = idx_;
                g.forward_edges = fwd_; g.backward_edges = bwd_;
                g.start_steps = start_steps_; g.start_relation = start_rel_;
                return g;
            }
            [[nodiscard]] std::string error() const { return err_; }
        };

        [[nodiscard]] static Builder builder(std::string id, std::string name) { return Builder(std::move(id), std::move(name)); }
    };

}  // namespace ospf::framework::gantt
