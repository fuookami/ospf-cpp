/// ospf_framework_gantt 测试
/// 覆盖 Gantt 排程领域模型、排程算法、上下文。

#include <gtest/gtest.h>

#include <ospf/framework/gantt/domain.hpp>
#include <ospf/framework/gantt/application.hpp>
#include <ospf/framework/gantt/context.hpp>

using namespace ospf::framework::gantt;

// ============================================================================
// Domain 测试 / Domain tests
// ============================================================================

TEST(Domain, Task) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.id, "t1");
    EXPECT_EQ(task.name, "Task 1");
    EXPECT_DOUBLE_EQ(task.duration, 5.0);
    EXPECT_DOUBLE_EQ(task.earliest_end(), 5.0);
}

TEST(Domain, TaskWithPredecessors) {
    Task task{"t2", "Task 2", 3.0, 5.0, 15.0, {"t1"}, "r1"};
    EXPECT_EQ(task.predecessors.size(), 1u);
    EXPECT_EQ(task.predecessors[0], "t1");
}

TEST(Domain, Resource) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_EQ(res.id, "r1");
    EXPECT_EQ(res.name, "Machine 1");
    EXPECT_DOUBLE_EQ(res.capacity, 2.0);
}

TEST(Domain, Capacity) {
    Capacity cap{"r1", 0.0, 1.0, 2.0};
    EXPECT_DOUBLE_EQ(cap.available(), 1.0);
    EXPECT_FALSE(cap.is_full());

    cap.used = 2.0;
    EXPECT_DOUBLE_EQ(cap.available(), 0.0);
    EXPECT_TRUE(cap.is_full());
}

TEST(Domain, Bunch) {
    Bunch bunch{"b1", {"t1", "t2", "t3"}};
    EXPECT_EQ(bunch.id, "b1");
    EXPECT_EQ(bunch.size(), 3u);
}

TEST(Domain, ScheduleResult) {
    ScheduleResult result;
    result.scheduled_tasks = {
        {"t1", 0.0, 5.0, "r1"},
        {"t2", 5.0, 8.0, "r1"}
    };
    result.makespan = 8.0;

    EXPECT_DOUBLE_EQ(result.get_start("t1"), 0.0);
    EXPECT_DOUBLE_EQ(result.get_end("t2"), 8.0);
    EXPECT_DOUBLE_EQ(result.makespan, 8.0);
}

// ============================================================================
// Application 测试 / Application tests
// ============================================================================

TEST(Application, GreedySchedulerName) {
    GreedyScheduler scheduler;
    EXPECT_EQ(scheduler.name(), "GreedyScheduler");
}

TEST(Application, GreedySchedulerBasic) {
    GreedyScheduler scheduler;

    std::vector<Task> tasks = {
        {"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"},
        {"t2", "Task 2", 3.0, 0.0, 100.0, {}, "r1"},
        {"t3", "Task 3", 4.0, 0.0, 100.0, {"t1"}, "r1"}
    };

    std::vector<Resource> resources = {
        {"r1", "Machine 1", 1.0}
    };

    auto result = scheduler.schedule(tasks, resources);

    EXPECT_EQ(result.scheduled_tasks.size(), 3u);
    EXPECT_GT(result.makespan, 0.0);
}

TEST(Application, GreedySchedulerWithPredecessors) {
    GreedyScheduler scheduler;

    std::vector<Task> tasks = {
        {"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"},
        {"t2", "Task 2", 3.0, 0.0, 100.0, {"t1"}, "r1"}
    };

    std::vector<Resource> resources = {
        {"r1", "Machine 1", 1.0}
    };

    auto result = scheduler.schedule(tasks, resources);

    // t2 应该在 t1 之后开始
    auto t1_end = result.get_end("t1");
    auto t2_start = result.get_start("t2");
    EXPECT_GE(t2_start, t1_end);
}

TEST(Application, GreedySchedulerEmpty) {
    GreedyScheduler scheduler;
    std::vector<Task> tasks;
    std::vector<Resource> resources;

    auto result = scheduler.schedule(tasks, resources);
    EXPECT_EQ(result.scheduled_tasks.size(), 0u);
    EXPECT_DOUBLE_EQ(result.makespan, 0.0);
}

TEST(Application, BranchAndPriceName) {
    BranchAndPriceSolver solver;
    EXPECT_EQ(solver.name(), "BranchAndPrice");
}

TEST(Application, BranchAndPriceBasic) {
    BranchAndPriceSolver solver;

    std::vector<Task> tasks = {
        {"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"},
        {"t2", "Task 2", 3.0, 0.0, 100.0, {}, "r1"}
    };

    std::vector<Resource> resources = {
        {"r1", "Machine 1", 1.0}
    };

    auto result = solver.solve(tasks, resources);
    EXPECT_EQ(result.scheduled_tasks.size(), 2u);
}

// ============================================================================
// Context 测试 / Context tests
// ============================================================================

TEST(GanttContext, Creation) {
    GanttContext ctx;
    EXPECT_EQ(ctx.domain_name(), "Gantt");
}

TEST(GanttContext, AddTask) {
    GanttContext ctx;
    ctx.add_task({"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"});
    EXPECT_EQ(ctx.tasks().size(), 1u);
}

TEST(GanttContext, AddResource) {
    GanttContext ctx;
    ctx.add_resource({"r1", "Machine 1", 2.0});
    EXPECT_EQ(ctx.resources().size(), 1u);
}

TEST(GanttContext, Solve) {
    GanttContext ctx;
    ctx.add_task({"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"});
    ctx.add_task({"t2", "Task 2", 3.0, 0.0, 100.0, {}, "r1"});
    ctx.add_resource({"r1", "Machine 1", 1.0});

    GreedyScheduler scheduler;
    auto result = ctx.solve(scheduler);

    EXPECT_EQ(result.scheduled_tasks.size(), 2u);
    EXPECT_GT(result.makespan, 0.0);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, GanttWorkflow) {
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

    // 执行排程
    GreedyScheduler scheduler;
    auto result = ctx.solve(scheduler);

    // 验证结果
    EXPECT_EQ(result.scheduled_tasks.size(), 4u);
    EXPECT_GT(result.makespan, 0.0);

    // 验证依赖关系
    auto cut_end = result.get_end("cut");
    auto drill_start = result.get_start("drill");
    EXPECT_GE(drill_start, cut_end);

    auto drill_end = result.get_end("drill");
    auto assemble_start = result.get_start("assemble");
    EXPECT_GE(assemble_start, drill_end);
}
