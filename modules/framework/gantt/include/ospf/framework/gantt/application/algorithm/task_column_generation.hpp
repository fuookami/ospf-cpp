#pragma once
/// Gantt application algorithm/task_column_generation /// 1:1 对应 Rust gantt/application/algorithm/task_column_generation.rs
/// 任务级列生成：TaskColumnGeneration 控制流

#include <string>
#include <vector>

namespace ospf::framework::gantt {

    /// 任务列生成配置 / Task column generation config
    struct TaskColumnGenerationConfig {
        int max_candidates = 10;
        double reduced_cost_tolerance = 1e-6;
    };

    /// 任务列生成结果 / Task column generation result
    struct TaskColumnGenerationResult {
        std::vector<std::string> generated_tasks;
        int iteration = 0;
        bool converged = false;
    };

}  // namespace ospf::framework::gantt
