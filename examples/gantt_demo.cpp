/// Gantt demo
/// 演示排程问题的求解。

#include <ospf/framework/gantt/context.hpp>
#include <ospf/framework/gantt/application.hpp>
#include <iostream>

int main() {
    using namespace ospf::framework::gantt;

    std::cout << "=== Gantt Scheduling Demo ===" << std::endl;

    // 创建上下文
    GanttContext ctx;

    // 添加资源
    ctx.add_resource({"machine1", "CNC Machine 1", 1.0});
    ctx.add_resource({"machine2", "CNC Machine 2", 1.0});

    // 添加任务（带有依赖关系）
    ctx.add_task({"cut", "Cut Material", 2.0, 0.0, 100.0, {}, "machine1"});
    ctx.add_task({"drill", "Drill Holes", 1.5, 0.0, 100.0, {"cut"}, "machine2"});
    ctx.add_task({"assemble", "Assemble", 3.0, 0.0, 100.0, {"drill"}, "machine1"});
    ctx.add_task({"paint", "Paint", 2.0, 0.0, 100.0, {"assemble"}, "machine2"});

    std::cout << "Tasks: " << ctx.tasks().size() << std::endl;
    std::cout << "Resources: " << ctx.resources().size() << std::endl;

    // 执行排程
    GreedyScheduler scheduler;
    auto result = ctx.solve(scheduler);

    std::cout << "Algorithm: " << scheduler.name() << std::endl;
    std::cout << "Scheduled tasks: " << result.scheduled_tasks.size() << std::endl;
    std::cout << "Makespan: " << result.makespan << std::endl;

    std::cout << "\nSchedule:" << std::endl;
    for (const auto& st : result.scheduled_tasks) {
        std::cout << "  " << st.task_id
                  << ": [" << st.start_time << " - " << st.end_time << "]"
                  << " on " << st.resource_id << std::endl;
    }

    std::cout << "=== Demo Complete ===" << std::endl;
    return 0;
}
