#pragma once

/// Gantt 排程领域模型 / Gantt scheduling domain model
/// Task、Bunch、Capacity、Resource。

#include <cstdint>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

namespace ospf::framework::gantt {

    /// 任务 / Task
    struct Task {
        std::string id;
        std::string name;
        double duration = 0.0;       ///< 持续时间 / Duration
        double earliest_start = 0.0;  ///< 最早开始时间 / Earliest start
        double latest_end = 0.0;      ///< 最晚结束时间 / Latest end
        std::vector<std::string> predecessors;  ///< 前置任务 / Predecessors
        std::string resource_id;      ///< 所需资源 / Required resource

        [[nodiscard]] double earliest_end() const noexcept {
            return earliest_start + duration;
        }
    };

    /// 资源 / Resource
    struct Resource {
        std::string id;
        std::string name;
        double capacity = 1.0;  ///< 容量 / Capacity (how many tasks can run in parallel)
    };

    /// 资源容量 / Resource capacity at time point
    struct Capacity {
        std::string resource_id;
        double time = 0.0;
        double used = 0.0;
        double total = 1.0;

        [[nodiscard]] double available() const noexcept { return total - used; }
        [[nodiscard]] bool is_full() const noexcept { return used >= total; }
    };

    /// 任务批次 / Bunch (group of tasks)
    struct Bunch {
        std::string id;
        std::vector<std::string> task_ids;

        [[nodiscard]] std::size_t size() const noexcept { return task_ids.size(); }
    };

    /// 排程结果 / Schedule result
    struct ScheduleResult {
        struct ScheduledTask {
            std::string task_id;
            double start_time = 0.0;
            double end_time = 0.0;
            std::string resource_id;
        };

        std::vector<ScheduledTask> scheduled_tasks;
        double makespan = 0.0;  ///< 总完工时间 / Total makespan

        [[nodiscard]] double get_start(const std::string& task_id) const {
            for (const auto& st : scheduled_tasks) {
                if (st.task_id == task_id) return st.start_time;
            }
            return 0.0;
        }

        [[nodiscard]] double get_end(const std::string& task_id) const {
            for (const auto& st : scheduled_tasks) {
                if (st.task_id == task_id) return st.end_time;
            }
            return 0.0;
        }
    };

}  // namespace ospf::framework::gantt
