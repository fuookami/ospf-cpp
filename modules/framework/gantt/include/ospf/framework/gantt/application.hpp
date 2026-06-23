#pragma once

/// Gantt 排程应用层 / Gantt scheduling application layer
/// 排程算法和分支定价。

#include <ospf/framework/gantt/domain.hpp>
#include <algorithm>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

namespace ospf::framework::gantt {

    /// 排程算法基类 / Scheduling algorithm base class
    class SchedulingAlgorithm {
    public:
        virtual ~SchedulingAlgorithm() = default;

        /// 执行排程 / Execute scheduling
        [[nodiscard]] virtual ScheduleResult schedule(
            const std::vector<Task>& tasks,
            const std::vector<Resource>& resources) = 0;

        /// 算法名称 / Algorithm name
        [[nodiscard]] virtual const std::string& name() const = 0;
    };

    /// 贪心排程算法 / Greedy scheduling algorithm
    class GreedyScheduler : public SchedulingAlgorithm {
    public:
        [[nodiscard]] ScheduleResult schedule(
            const std::vector<Task>& tasks,
            const std::vector<Resource>& resources) override
        {
            ScheduleResult result;

            // 构建资源容量表
            std::unordered_map<std::string, double> resource_capacity;
            for (const auto& res : resources) {
                resource_capacity[res.id] = res.capacity;
            }

            // 计算每个任务的最早开始时间（考虑前置任务）
            std::unordered_map<std::string, double> earliest_start;
            std::unordered_map<std::string, double> task_end;

            // 拓扑排序
            auto sorted = topological_sort(tasks);

            for (const auto& task : sorted) {
                double est = task.earliest_start;

                // 考虑前置任务的结束时间
                for (const auto& pred : task.predecessors) {
                    if (task_end.count(pred)) {
                        est = std::max(est, task_end[pred]);
                    }
                }

                earliest_start[task.id] = est;
                task_end[task.id] = est + task.duration;

                ScheduleResult::ScheduledTask st;
                st.task_id = task.id;
                st.start_time = est;
                st.end_time = est + task.duration;
                st.resource_id = task.resource_id;
                result.scheduled_tasks.push_back(st);

                result.makespan = std::max(result.makespan, st.end_time);
            }

            return result;
        }

        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "GreedyScheduler";
            return n;
        }

    private:
        /// 拓扑排序 / Topological sort
        [[nodiscard]] static std::vector<Task> topological_sort(const std::vector<Task>& tasks) {
            // 简化：按 earliest_start 排序
            std::vector<Task> sorted = tasks;
            std::sort(sorted.begin(), sorted.end(),
                [](const Task& a, const Task& b) {
                    return a.earliest_start < b.earliest_start;
                });
            return sorted;
        }
    };

    /// 分支定价求解器 / Branch and Price solver
    class BranchAndPriceSolver {
    public:
        struct BranchNode {
            std::vector<std::string> fixed_tasks;
            double lower_bound = 0.0;
            double upper_bound = std::numeric_limits<double>::infinity();
        };

        /// 求解 / Solve
        [[nodiscard]] ScheduleResult solve(
            const std::vector<Task>& tasks,
            const std::vector<Resource>& resources)
        {
            // 简化实现：使用贪心算法作为初始解
            GreedyScheduler scheduler;
            return scheduler.schedule(tasks, resources);
        }

        [[nodiscard]] const std::string& name() const {
            static const std::string n = "BranchAndPrice";
            return n;
        }
    };

}  // namespace ospf::framework::gantt
