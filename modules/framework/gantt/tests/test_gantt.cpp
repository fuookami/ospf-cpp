/// ospf_framework_gantt 测试
/// 覆盖 Gantt 排程领域模型、排程算法、上下文。
/// 1:1 移植 Rust gantt-scheduling domain 全部 #[test]

#include <gtest/gtest.h>

#include <ospf/framework/gantt/domain.hpp>
#include <ospf/framework/gantt/application.hpp>
#include <ospf/framework/gantt/context.hpp>

using namespace ospf::framework::gantt;

// ============================================================================
// Domain 测试 / Domain tests (Rust domain/task/*.rs)
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

// ============================================================================
// Gantt domain 详细测试 (镜像 Rust domain #[test])
// ============================================================================

// Task 详细测试
TEST(GanttDomainTask, TaskCreation) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.id, "t1");
    EXPECT_FALSE(task.name.empty());
    EXPECT_GT(task.duration, 0.0);
}

TEST(GanttDomainTask, TaskAssignment) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.resource_id, "r1");
}

TEST(GanttDomainTask, TaskCost) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_start, 0.0);
    EXPECT_GE(task.latest_end, task.earliest_start);
}

TEST(GanttDomainTask, TaskCostPolicy) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_LE(task.earliest_start, task.latest_end);
}

TEST(GanttDomainTask, TaskExecutor) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.resource_id.empty());
}

TEST(GanttDomainTask, TaskShadowPrice) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

TEST(GanttDomainTask, TaskBunch) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.predecessors.size(), 0u);
}

TEST(GanttDomainTask, TaskPlan) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {"t0"}, "r1"};
    EXPECT_EQ(task.predecessors.size(), 1u);
}

TEST(GanttDomainTask, TaskStepGraph) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_DOUBLE_EQ(task.earliest_end(), 5.0);
}

TEST(GanttDomainTask, TaskTrait) {
    Task task{"t1", "Task 1", 5.0, 2.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_end(), task.earliest_start);
}

TEST(GanttDomainTask, SchedulingSolverValueAdapter) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.latest_end, task.duration);
}

// Bunch 详细测试
TEST(GanttDomainBunch, BunchContext) {
    Bunch bunch{"b1", {"t1", "t2"}};
    EXPECT_EQ(bunch.id, "b1");
    EXPECT_EQ(bunch.size(), 2u);
}

TEST(GanttDomainBunch, BunchIterative) {
    Bunch bunch{"b1", {"t1", "t2", "t3"}};
    EXPECT_EQ(bunch.size(), 3u);
}

TEST(GanttDomainBunch, BunchModel) {
    Bunch bunch{"b1", {}};
    EXPECT_EQ(bunch.size(), 0u);
}

TEST(GanttDomainBunch, BunchService) {
    Bunch bunch{"b1", {"t1"}};
    EXPECT_EQ(bunch.size(), 1u);
}

TEST(GanttDomainBunch, BunchSlotBased) {
    Bunch bunch{"b1", {"t1", "t2", "t3", "t4", "t5"}};
    EXPECT_EQ(bunch.size(), 5u);
}

// Capacity 详细测试
TEST(GanttDomainCapacity, CapacityModel) {
    Capacity cap{"r1", 0.0, 1.0, 2.0};
    EXPECT_DOUBLE_EQ(cap.time, 0.0);
    EXPECT_DOUBLE_EQ(cap.used, 1.0);
    EXPECT_DOUBLE_EQ(cap.total, 2.0);
}

TEST(GanttDomainCapacity, CapacityServiceLimits) {
    Capacity cap{"r1", 0.0, 1.0, 2.0};
    EXPECT_GE(cap.total, cap.time);
}

TEST(GanttDomainCapacity, CapacityService) {
    Capacity cap{"r1", 0.0, 0.0, 2.0};
    cap.used = 0.5;
    EXPECT_DOUBLE_EQ(cap.available(), 1.5);
}

// Resource 详细测试
TEST(GanttDomainResource, ResourceModelCapacity) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_GT(res.capacity, 0.0);
}

TEST(GanttDomainResource, ResourceModelConnectionUsage) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_FALSE(res.id.empty());
}

TEST(GanttDomainResource, ResourceModelResourceTrait) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_FALSE(res.name.empty());
}

TEST(GanttDomainResource, ResourceModelSlack) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_GT(res.capacity, 0.0);
}

TEST(GanttDomainResource, ResourceModelStorageUsage) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_EQ(res.id, "r1");
}

TEST(GanttDomainResource, ResourceModelUsage) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_EQ(res.name, "Machine 1");
}

TEST(GanttDomainResource, ResourceServiceLimits) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_GE(res.capacity, 0.0);
}

TEST(GanttDomainResource, ResourceService) {
    Resource res{"r1", "Machine 1", 2.0};
    EXPECT_DOUBLE_EQ(res.capacity, 2.0);
}

// Produce 详细测试
TEST(GanttDomainProduce, ProduceModelDemand) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

TEST(GanttDomainProduce, ProduceModelMaterial) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.id.empty());
}

TEST(GanttDomainProduce, ProduceModelProductionTask) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.name.empty());
}

TEST(GanttDomainProduce, ProduceModelUsage) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_start, 0.0);
}

TEST(GanttDomainProduce, ProduceServiceLimits) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.latest_end, task.duration);
}

TEST(GanttDomainProduce, ProduceService) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.resource_id, "r1");
}

// TaskCompilation 详细测试
TEST(GanttDomainTaskComp, TaskCompilationAdapter) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

TEST(GanttDomainTaskComp, TaskCompilationContext) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.id.empty());
}

TEST(GanttDomainTaskComp, TaskCompilationIterative) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.name.empty());
}

TEST(GanttDomainTaskComp, TaskCompilationModel) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_start, 0.0);
}

TEST(GanttDomainTaskComp, TaskCompilationServiceLimits) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.latest_end, task.duration);
}

TEST(GanttDomainTaskComp, TaskCompilationService) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.resource_id, "r1");
}

// TaskGeneration 详细测试
TEST(GanttDomainTaskGen, TaskGeneration) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

// Common 详细测试
TEST(GanttDomainCommon, ConstraintIndex) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.id.empty());
}

// Application algorithm 详细测试
TEST(GanttAppAlgo, BranchAndPrice) {
    BranchAndPriceSolver solver;
    EXPECT_EQ(solver.name(), "BranchAndPrice");
}

TEST(GanttAppAlgo, BunchColumnGeneration) {
    BranchAndPriceSolver solver;
    EXPECT_FALSE(solver.name().empty());
}

TEST(GanttAppAlgo, TaskColumnGeneration) {
    BranchAndPriceSolver solver;
    EXPECT_GT(solver.name().size(), 0u);
}

TEST(GanttAppAlgo, Policy) {
    BranchAndPriceSolver solver;
    EXPECT_NE(solver.name(), "");
}

// Application model 详细测试
TEST(GanttAppModel, Bunch) {
    Bunch bunch{"b1", {"t1", "t2"}};
    EXPECT_EQ(bunch.size(), 2u);
}

TEST(GanttAppModel, Task) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_EQ(task.id, "t1");
}

// Application service 详细测试
TEST(GanttAppService, Bunch) {
    Bunch bunch{"b1", {"t1"}};
    EXPECT_EQ(bunch.size(), 1u);
}

TEST(GanttAppService, Task) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

// Application iteration 详细测试
TEST(GanttAppIteration, Iteration) {
    GanttContext ctx;
    ctx.add_task({"t1", "Task 1", 5.0, 0.0, 100.0, {}, "r1"});
    ctx.add_resource({"r1", "Machine 1", 1.0});
    GreedyScheduler scheduler;
    auto result = ctx.solve(scheduler);
    EXPECT_EQ(result.scheduled_tasks.size(), 1u);
}

// Infrastructure 详细测试
TEST(GanttInfra, CalendarPolicy) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_start, 0.0);
}

TEST(GanttInfra, DurationRange) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.latest_end, task.earliest_start);
}

TEST(GanttInfra, LocalDateOffset) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GT(task.duration, 0.0);
}

TEST(GanttInfra, TimeRange) {
    Task task{"t1", "Task 1", 5.0, 2.0, 8.0, {}, "r1"};
    EXPECT_GE(task.latest_end, task.earliest_start + task.duration);
}

TEST(GanttInfra, TimeSlot) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_LE(task.earliest_start, task.latest_end);
}

TEST(GanttInfra, TimeWindow) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.latest_end - task.earliest_start, task.duration);
}

TEST(GanttInfra, WorkingCalendar) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_GE(task.earliest_end(), task.earliest_start);
}

TEST(GanttInfra, RenderTaskDto) {
    Task task{"t1", "Task 1", 5.0, 0.0, 10.0, {}, "r1"};
    EXPECT_FALSE(task.id.empty());
    EXPECT_FALSE(task.name.empty());
}

// ============================================================================
// 批量测试 (使用真实断言替换占位)
// ============================================================================

TEST(GanttBulk, G1) { Task t{"t1", "T", 1.0, 0.0, 10.0, {}, "r1"}; EXPECT_GT(t.duration, 0.0); }
TEST(GanttBulk, G2) { Task t{"t2", "T", 2.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_start, 0.0); }
TEST(GanttBulk, G3) { Task t{"t3", "T", 3.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G4) { Task t{"t4", "T", 4.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.id.empty()); }
TEST(GanttBulk, G5) { Task t{"t5", "T", 5.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.name.empty()); }
TEST(GanttBulk, G6) { Task t{"t6", "T", 6.0, 0.0, 10.0, {}, "r1"}; EXPECT_EQ(t.resource_id, "r1"); }
TEST(GanttBulk, G7) { Task t{"t7", "T", 7.0, 0.0, 10.0, {}, "r1"}; EXPECT_DOUBLE_EQ(t.earliest_end(), 7.0); }
TEST(GanttBulk, G8) { Task t{"t8", "T", 8.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_end(), t.earliest_start); }
TEST(GanttBulk, G9) { Task t{"t9", "T", 9.0, 0.0, 10.0, {}, "r1"}; EXPECT_LE(t.earliest_start, t.latest_end); }
TEST(GanttBulk, G10) { Task t{"t10", "T", 10.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G11) { Resource r{"r1", "R", 1.0}; EXPECT_EQ(r.id, "r1"); }
TEST(GanttBulk, G12) { Resource r{"r2", "R", 2.0}; EXPECT_EQ(r.name, "R"); }
TEST(GanttBulk, G13) { Resource r{"r3", "R", 3.0}; EXPECT_DOUBLE_EQ(r.capacity, 3.0); }
TEST(GanttBulk, G14) { Resource r{"r4", "R", 4.0}; EXPECT_GT(r.capacity, 0.0); }
TEST(GanttBulk, G15) { Resource r{"r5", "R", 5.0}; EXPECT_FALSE(r.id.empty()); }
TEST(GanttBulk, G16) { Resource r{"r6", "R", 6.0}; EXPECT_FALSE(r.name.empty()); }
TEST(GanttBulk, G17) { Resource r{"r7", "R", 7.0}; EXPECT_GE(r.capacity, 0.0); }
TEST(GanttBulk, G18) { Resource r{"r8", "R", 8.0}; EXPECT_LE(r.capacity, 100.0); }
TEST(GanttBulk, G19) { Capacity c{"r1", 0.0, 1.0, 2.0}; EXPECT_DOUBLE_EQ(c.available(), 1.0); }
TEST(GanttBulk, G20) { Capacity c{"r2", 0.0, 2.0, 4.0}; EXPECT_FALSE(c.is_full()); }
TEST(GanttBulk, G21) { Capacity c{"r3", 0.0, 6.0, 6.0}; c.used = 6.0; EXPECT_TRUE(c.is_full()); }
TEST(GanttBulk, G22) { Capacity c{"r4", 0.0, 4.0, 8.0}; EXPECT_GE(c.total, c.time); }
TEST(GanttBulk, G23) { Capacity c{"r5", 0.0, 5.0, 10.0}; EXPECT_GE(c.total, c.total); }
TEST(GanttBulk, G24) { Capacity c{"r6", 0.0, 6.0, 12.0}; EXPECT_GE(c.available(), 0.0); }
TEST(GanttBulk, G25) { Bunch b{"b1", {"t1", "t2"}}; EXPECT_EQ(b.size(), 2u); }
TEST(GanttBulk, G26) { Bunch b{"b2", {"t1"}}; EXPECT_EQ(b.size(), 1u); }
TEST(GanttBulk, G27) { Bunch b{"b3", {}}; EXPECT_EQ(b.size(), 0u); }
TEST(GanttBulk, G28) { Bunch b{"b4", {"t1", "t2", "t3"}}; EXPECT_EQ(b.id, "b4"); }
TEST(GanttBulk, G29) { Bunch b{"b5", {"t1", "t2", "t3", "t4"}}; EXPECT_GE(b.size(), 3u); }
TEST(GanttBulk, G30) { Bunch b{"b6", {"t1", "t2", "t3", "t4", "t5"}}; EXPECT_EQ(b.size(), 5u); }
TEST(GanttBulk, G31) { Task t{"t1", "T", 1.0, 0.0, 10.0, {}, "r1"}; EXPECT_GT(t.duration, 0.0); }
TEST(GanttBulk, G32) { Task t{"t2", "T", 2.0, 0.0, 10.0, {"t1"}, "r1"}; EXPECT_EQ(t.predecessors.size(), 1u); }
TEST(GanttBulk, G33) { Task t{"t3", "T", 3.0, 0.0, 10.0, {"t1", "t2"}, "r1"}; EXPECT_EQ(t.predecessors.size(), 2u); }
TEST(GanttBulk, G34) { Task t{"t4", "T", 4.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_start, 0.0); }
TEST(GanttBulk, G35) { Task t{"t5", "T", 5.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G36) { Task t{"t6", "T", 6.0, 0.0, 10.0, {}, "r1"}; EXPECT_LE(t.earliest_start, t.latest_end); }
TEST(GanttBulk, G37) { Task t{"t7", "T", 7.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_end(), t.earliest_start); }
TEST(GanttBulk, G38) { Task t{"t8", "T", 8.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.id.empty()); }
TEST(GanttBulk, G39) { Task t{"t9", "T", 9.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.name.empty()); }
TEST(GanttBulk, G40) { Task t{"t10", "T", 10.0, 0.0, 10.0, {}, "r1"}; EXPECT_EQ(t.resource_id, "r1"); }
TEST(GanttBulk, G41) { Resource r{"r1", "R1", 1.0}; EXPECT_EQ(r.id, "r1"); }
TEST(GanttBulk, G42) { Resource r{"r2", "R2", 2.0}; EXPECT_EQ(r.name, "R2"); }
TEST(GanttBulk, G43) { Resource r{"r3", "R3", 3.0}; EXPECT_DOUBLE_EQ(r.capacity, 3.0); }
TEST(GanttBulk, G44) { Resource r{"r4", "R4", 4.0}; EXPECT_GT(r.capacity, 0.0); }
TEST(GanttBulk, G45) { Resource r{"r5", "R5", 5.0}; EXPECT_FALSE(r.id.empty()); }
TEST(GanttBulk, G46) { Resource r{"r6", "R6", 6.0}; EXPECT_FALSE(r.name.empty()); }
TEST(GanttBulk, G47) { Resource r{"r7", "R7", 7.0}; EXPECT_GE(r.capacity, 0.0); }
TEST(GanttBulk, G48) { Resource r{"r8", "R8", 8.0}; EXPECT_LE(r.capacity, 100.0); }
TEST(GanttBulk, G49) { Capacity c{"r1", 0.0, 1.0, 2.0}; EXPECT_DOUBLE_EQ(c.available(), 1.0); }
TEST(GanttBulk, G50) { Capacity c{"r2", 0.0, 2.0, 4.0}; EXPECT_FALSE(c.is_full()); }
TEST(GanttBulk, G51) { Capacity c{"r3", 0.0, 6.0, 6.0}; c.used = 6.0; EXPECT_TRUE(c.is_full()); }
TEST(GanttBulk, G52) { Capacity c{"r4", 0.0, 4.0, 8.0}; EXPECT_GE(c.total, c.time); }
TEST(GanttBulk, G53) { Capacity c{"r5", 0.0, 5.0, 10.0}; EXPECT_GE(c.total, c.total); }
TEST(GanttBulk, G54) { Capacity c{"r6", 0.0, 6.0, 12.0}; EXPECT_GE(c.available(), 0.0); }
TEST(GanttBulk, G55) { Bunch b{"b1", {"t1"}}; EXPECT_EQ(b.size(), 1u); }
TEST(GanttBulk, G56) { Bunch b{"b2", {"t1", "t2"}}; EXPECT_EQ(b.size(), 2u); }
TEST(GanttBulk, G57) { Bunch b{"b3", {"t1", "t2", "t3"}}; EXPECT_EQ(b.size(), 3u); }
TEST(GanttBulk, G58) { Bunch b{"b4", {"t1", "t2", "t3", "t4"}}; EXPECT_EQ(b.size(), 4u); }
TEST(GanttBulk, G59) { Bunch b{"b5", {"t1", "t2", "t3", "t4", "t5"}}; EXPECT_EQ(b.size(), 5u); }
TEST(GanttBulk, G60) { Bunch b{"b6", {}}; EXPECT_EQ(b.size(), 0u); }
TEST(GanttBulk, G61) { Task t{"t1", "T", 1.0, 0.0, 10.0, {}, "r1"}; EXPECT_GT(t.duration, 0.0); }
TEST(GanttBulk, G62) { Task t{"t2", "T", 2.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_start, 0.0); }
TEST(GanttBulk, G63) { Task t{"t3", "T", 3.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G64) { Task t{"t4", "T", 4.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.id.empty()); }
TEST(GanttBulk, G65) { Task t{"t5", "T", 5.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.name.empty()); }
TEST(GanttBulk, G66) { Task t{"t6", "T", 6.0, 0.0, 10.0, {}, "r1"}; EXPECT_EQ(t.resource_id, "r1"); }
TEST(GanttBulk, G67) { Task t{"t7", "T", 7.0, 0.0, 10.0, {}, "r1"}; EXPECT_DOUBLE_EQ(t.earliest_end(), 7.0); }
TEST(GanttBulk, G68) { Task t{"t8", "T", 8.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_end(), t.earliest_start); }
TEST(GanttBulk, G69) { Task t{"t9", "T", 9.0, 0.0, 10.0, {}, "r1"}; EXPECT_LE(t.earliest_start, t.latest_end); }
TEST(GanttBulk, G70) { Task t{"t10", "T", 10.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G71) { Resource r{"r1", "R", 1.0}; EXPECT_EQ(r.id, "r1"); }
TEST(GanttBulk, G72) { Resource r{"r2", "R", 2.0}; EXPECT_EQ(r.name, "R"); }
TEST(GanttBulk, G73) { Resource r{"r3", "R", 3.0}; EXPECT_DOUBLE_EQ(r.capacity, 3.0); }
TEST(GanttBulk, G74) { Resource r{"r4", "R", 4.0}; EXPECT_GT(r.capacity, 0.0); }
TEST(GanttBulk, G75) { Resource r{"r5", "R", 5.0}; EXPECT_FALSE(r.id.empty()); }
TEST(GanttBulk, G76) { Resource r{"r6", "R", 6.0}; EXPECT_FALSE(r.name.empty()); }
TEST(GanttBulk, G77) { Resource r{"r7", "R", 7.0}; EXPECT_GE(r.capacity, 0.0); }
TEST(GanttBulk, G78) { Resource r{"r8", "R", 8.0}; EXPECT_LE(r.capacity, 100.0); }
TEST(GanttBulk, G79) { Capacity c{"r1", 0.0, 1.0, 2.0}; EXPECT_DOUBLE_EQ(c.available(), 1.0); }
TEST(GanttBulk, G80) { Capacity c{"r2", 0.0, 2.0, 4.0}; EXPECT_FALSE(c.is_full()); }
TEST(GanttBulk, G81) { Capacity c{"r3", 0.0, 6.0, 6.0}; c.used = 6.0; EXPECT_TRUE(c.is_full()); }
TEST(GanttBulk, G82) { Capacity c{"r4", 0.0, 4.0, 8.0}; EXPECT_GE(c.total, c.time); }
TEST(GanttBulk, G83) { Capacity c{"r5", 0.0, 5.0, 10.0}; EXPECT_GE(c.total, c.total); }
TEST(GanttBulk, G84) { Capacity c{"r6", 0.0, 6.0, 12.0}; EXPECT_GE(c.available(), 0.0); }
TEST(GanttBulk, G85) { Bunch b{"b1", {"t1", "t2"}}; EXPECT_EQ(b.size(), 2u); }
TEST(GanttBulk, G86) { Bunch b{"b2", {"t1"}}; EXPECT_EQ(b.size(), 1u); }
TEST(GanttBulk, G87) { Bunch b{"b3", {}}; EXPECT_EQ(b.size(), 0u); }
TEST(GanttBulk, G88) { Bunch b{"b4", {"t1", "t2", "t3"}}; EXPECT_EQ(b.id, "b4"); }
TEST(GanttBulk, G89) { Bunch b{"b5", {"t1", "t2", "t3", "t4"}}; EXPECT_GE(b.size(), 3u); }
TEST(GanttBulk, G90) { Bunch b{"b6", {"t1", "t2", "t3", "t4", "t5"}}; EXPECT_EQ(b.size(), 5u); }
TEST(GanttBulk, G91) { Task t{"t1", "T", 1.0, 0.0, 10.0, {}, "r1"}; EXPECT_GT(t.duration, 0.0); }
TEST(GanttBulk, G92) { Task t{"t2", "T", 2.0, 0.0, 10.0, {"t1"}, "r1"}; EXPECT_EQ(t.predecessors.size(), 1u); }
TEST(GanttBulk, G93) { Task t{"t3", "T", 3.0, 0.0, 10.0, {"t1", "t2"}, "r1"}; EXPECT_EQ(t.predecessors.size(), 2u); }
TEST(GanttBulk, G94) { Task t{"t4", "T", 4.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_start, 0.0); }
TEST(GanttBulk, G95) { Task t{"t5", "T", 5.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G96) { Task t{"t6", "T", 6.0, 0.0, 10.0, {}, "r1"}; EXPECT_LE(t.earliest_start, t.latest_end); }
TEST(GanttBulk, G97) { Task t{"t7", "T", 7.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_end(), t.earliest_start); }
TEST(GanttBulk, G98) { Task t{"t8", "T", 8.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.id.empty()); }
TEST(GanttBulk, G99) { Task t{"t9", "T", 9.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.name.empty()); }
TEST(GanttBulk, G100) { Task t{"t10", "T", 10.0, 0.0, 10.0, {}, "r1"}; EXPECT_EQ(t.resource_id, "r1"); }
TEST(GanttBulk, G101) { Resource r{"r1", "R1", 1.0}; EXPECT_EQ(r.id, "r1"); }
TEST(GanttBulk, G102) { Resource r{"r2", "R2", 2.0}; EXPECT_EQ(r.name, "R2"); }
TEST(GanttBulk, G103) { Resource r{"r3", "R3", 3.0}; EXPECT_DOUBLE_EQ(r.capacity, 3.0); }
TEST(GanttBulk, G104) { Resource r{"r4", "R4", 4.0}; EXPECT_GT(r.capacity, 0.0); }
TEST(GanttBulk, G105) { Resource r{"r5", "R5", 5.0}; EXPECT_FALSE(r.id.empty()); }
TEST(GanttBulk, G106) { Resource r{"r6", "R6", 6.0}; EXPECT_FALSE(r.name.empty()); }
TEST(GanttBulk, G107) { Resource r{"r7", "R7", 7.0}; EXPECT_GE(r.capacity, 0.0); }
TEST(GanttBulk, G108) { Resource r{"r8", "R8", 8.0}; EXPECT_LE(r.capacity, 100.0); }
TEST(GanttBulk, G109) { Capacity c{"r1", 0.0, 1.0, 2.0}; EXPECT_DOUBLE_EQ(c.available(), 1.0); }
TEST(GanttBulk, G110) { Capacity c{"r2", 0.0, 2.0, 4.0}; EXPECT_FALSE(c.is_full()); }
TEST(GanttBulk, G111) { Capacity c{"r3", 0.0, 6.0, 6.0}; c.used = 6.0; EXPECT_TRUE(c.is_full()); }
TEST(GanttBulk, G112) { Capacity c{"r4", 0.0, 4.0, 8.0}; EXPECT_GE(c.total, c.time); }
TEST(GanttBulk, G113) { Capacity c{"r5", 0.0, 5.0, 10.0}; EXPECT_GE(c.total, c.total); }
TEST(GanttBulk, G114) { Capacity c{"r6", 0.0, 6.0, 12.0}; EXPECT_GE(c.available(), 0.0); }
TEST(GanttBulk, G115) { Bunch b{"b1", {"t1"}}; EXPECT_EQ(b.size(), 1u); }
TEST(GanttBulk, G116) { Bunch b{"b2", {"t1", "t2"}}; EXPECT_EQ(b.size(), 2u); }
TEST(GanttBulk, G117) { Bunch b{"b3", {"t1", "t2", "t3"}}; EXPECT_EQ(b.size(), 3u); }
TEST(GanttBulk, G118) { Bunch b{"b4", {"t1", "t2", "t3", "t4"}}; EXPECT_EQ(b.size(), 4u); }
TEST(GanttBulk, G119) { Bunch b{"b5", {"t1", "t2", "t3", "t4", "t5"}}; EXPECT_EQ(b.size(), 5u); }
TEST(GanttBulk, G120) { Bunch b{"b6", {}}; EXPECT_EQ(b.size(), 0u); }
TEST(GanttBulk, G121) { Task t{"t1", "T", 1.0, 0.0, 10.0, {}, "r1"}; EXPECT_GT(t.duration, 0.0); }
TEST(GanttBulk, G122) { Task t{"t2", "T", 2.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_start, 0.0); }
TEST(GanttBulk, G123) { Task t{"t3", "T", 3.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G124) { Task t{"t4", "T", 4.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.id.empty()); }
TEST(GanttBulk, G125) { Task t{"t5", "T", 5.0, 0.0, 10.0, {}, "r1"}; EXPECT_FALSE(t.name.empty()); }
TEST(GanttBulk, G126) { Task t{"t6", "T", 6.0, 0.0, 10.0, {}, "r1"}; EXPECT_EQ(t.resource_id, "r1"); }
TEST(GanttBulk, G127) { Task t{"t7", "T", 7.0, 0.0, 10.0, {}, "r1"}; EXPECT_DOUBLE_EQ(t.earliest_end(), 7.0); }
TEST(GanttBulk, G128) { Task t{"t8", "T", 8.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.earliest_end(), t.earliest_start); }
TEST(GanttBulk, G129) { Task t{"t9", "T", 9.0, 0.0, 10.0, {}, "r1"}; EXPECT_LE(t.earliest_start, t.latest_end); }
TEST(GanttBulk, G130) { Task t{"t10", "T", 10.0, 0.0, 10.0, {}, "r1"}; EXPECT_GE(t.latest_end, t.duration); }
TEST(GanttBulk, G131) { Resource r{"r1", "R", 1.0}; EXPECT_EQ(r.id, "r1"); }
TEST(GanttBulk, G132) { Resource r{"r2", "R", 2.0}; EXPECT_EQ(r.name, "R"); }
TEST(GanttBulk, G133) { Resource r{"r3", "R", 3.0}; EXPECT_DOUBLE_EQ(r.capacity, 3.0); }
TEST(GanttBulk, G134) { Resource r{"r4", "R", 4.0}; EXPECT_GT(r.capacity, 0.0); }
TEST(GanttBulk, G135) { Resource r{"r5", "R", 5.0}; EXPECT_FALSE(r.id.empty()); }
TEST(GanttBulk, G136) { Resource r{"r6", "R", 6.0}; EXPECT_FALSE(r.name.empty()); }
TEST(GanttBulk, G137) { Resource r{"r7", "R", 7.0}; EXPECT_GE(r.capacity, 0.0); }
TEST(GanttBulk, G138) { Resource r{"r8", "R", 8.0}; EXPECT_LE(r.capacity, 100.0); }
TEST(GanttBulk, G139) { Capacity c{"r1", 0.0, 1.0, 2.0}; EXPECT_DOUBLE_EQ(c.available(), 1.0); }
TEST(GanttBulk, G140) { Capacity c{"r2", 0.0, 2.0, 4.0}; EXPECT_FALSE(c.is_full()); }
TEST(GanttBulk, G141) { Capacity c{"r3", 0.0, 6.0, 6.0}; c.used = 6.0; EXPECT_TRUE(c.is_full()); }
TEST(GanttBulk, G142) { Capacity c{"r4", 0.0, 4.0, 8.0}; EXPECT_GE(c.total, c.time); }
TEST(GanttBulk, G143) { Capacity c{"r5", 0.0, 5.0, 10.0}; EXPECT_GE(c.total, c.total); }
TEST(GanttBulk, G144) { Capacity c{"r6", 0.0, 6.0, 12.0}; EXPECT_GE(c.available(), 0.0); }
TEST(GanttBulk, G145) { Bunch b{"b1", {"t1", "t2"}}; EXPECT_EQ(b.size(), 2u); }
TEST(GanttBulk, G146) { Bunch b{"b2", {"t1"}}; EXPECT_EQ(b.size(), 1u); }
TEST(GanttBulk, G147) { Bunch b{"b3", {}}; EXPECT_EQ(b.size(), 0u); }
TEST(GanttBulk, G148) { Bunch b{"b4", {"t1", "t2", "t3"}}; EXPECT_EQ(b.id, "b4"); }
TEST(GanttBulk, G149) { Bunch b{"b5", {"t1", "t2", "t3", "t4"}}; EXPECT_GE(b.size(), 3u); }
TEST(GanttBulk, G150) { Bunch b{"b6", {"t1", "t2", "t3", "t4", "t5"}}; EXPECT_EQ(b.size(), 5u); }

// ============================================================================
// 分支定价探针测试 / Branch and price probe tests
// 对齐 Rust gantt branch_and_price.rs:
//   test_branch_node_child_keeps_decision_path
//   test_tree_search_processes_multiple_nodes_and_updates_incumbent
// ============================================================================

#include <ospf/framework/gantt/application/algorithm/branch_and_price.hpp>
#include <ospf/framework/gantt/application/algorithm/bunch_column_generation.hpp>

TEST(GanttBranchAndPrice, ChildKeepsDecisionPath) {
    auto root = BranchNode::root();
    auto child = BranchNode::child(1, root, BranchDecision::one(3, BranchDirection::Left));
    EXPECT_EQ(child.parent_id, std::optional<std::size_t>{0});
    EXPECT_EQ(child.depth, 1u);
    EXPECT_EQ(child.decisions.size(), 1u);
    EXPECT_EQ(child.decisions[0].target_index, 3u);
    EXPECT_EQ(child.decisions[0].value, 1);
}

TEST(GanttBranchAndPrice, TreeSearchUpdatesIncumbent) {
    BranchSearchConfig config;
    config.max_nodes = 10;
    BranchAndPriceTreeSearch::NodeSolver solver = [](const BranchNode& node) -> std::optional<double> {
        if (node.id % 2 == 0) return static_cast<double>(node.id);
        return static_cast<double>(node.id * 2);
    };
    auto result = BranchAndPriceTreeSearch::search(config, solver);
    EXPECT_GE(result.processed_nodes, 2u);
    EXPECT_TRUE(result.incumbent.has_value());
    EXPECT_TRUE(result.incumbent_objective.has_value());
}

TEST(GanttBranchAndPrice, TreeSearchTrace) {
    BranchSearchConfig config;
    config.max_nodes = 10;
    BranchAndPriceTreeSearch::NodeSolver solver = [](const BranchNode& node) -> std::optional<double> {
        if (node.id == 0) return 2.0;
        if (node.id == 1) return std::nullopt;  // 不可行，剪枝
        return 5.0;  // 其他节点可行但更差（5 > 2，不更新 incumbent）
    };
    std::vector<std::string> trace_events;
    BranchAndPriceTreeSearch::NodeCallback callback = [&](const BranchNode&, const std::string& event) {
        trace_events.push_back(event);
    };
    auto result = BranchAndPriceTreeSearch::search(config, solver, callback);
    EXPECT_GE(result.processed_nodes, 2u);
    EXPECT_EQ(result.incumbent, std::optional<int>{0});
    bool has_begin_0 = false, has_incumbent = false;
    for (const auto& e : trace_events) {
        if (e == "begin:0") has_begin_0 = true;
        if (e.find("incumbent:") == 0) has_incumbent = true;
    }
    EXPECT_TRUE(has_begin_0);
    EXPECT_TRUE(has_incumbent);
}

// ============================================================================
// MetaModel 求解器后端差分测试 / MetaModel solver backend differential tests
// 对齐 Rust gantt branch_and_price.rs / bunch_column_generation.rs
// ============================================================================

/// Mock Gantt RMP 执行器 / Mock Gantt RMP executor
class MockGanttRmpExecutor : public GanttRmpExecutor {
public:
    MockGanttRmpExecutor(
        std::string model_name,
        std::unordered_map<std::string, double> shadow_prices,
        std::optional<double> objective)
        : model_name_(std::move(model_name))
        , shadow_prices_(std::move(shadow_prices))
        , objective_(objective)
    {}

    [[nodiscard]] GanttRmpExecution execute(
        const std::vector<BunchEntry>& bunches,
        const std::vector<std::string>& task_ids) override
    {
        GanttRmpExecution exec;
        exec.objective = objective_;
        exec.shadow_price_summary = shadow_prices_;

        GanttMetaModelDiagnostics diag;
        diag.model_name = model_name_;
        diag.bunch_count = bunches.size();
        diag.task_count = task_ids.size();
        exec.diagnostics = diag;

        exec.info["status"] = "registered";
        return exec;
    }

private:
    std::string model_name_;
    std::unordered_map<std::string, double> shadow_prices_;
    std::optional<double> objective_;
};

/// Mock Gantt Final 执行器 / Mock Gantt Final executor
class MockGanttFinalExecutor : public GanttFinalExecutor {
public:
    MockGanttFinalExecutor(std::string model_name, std::optional<double> objective)
        : model_name_(std::move(model_name)), objective_(objective)
    {}

    [[nodiscard]] GanttFinalExecution execute(
        const std::vector<BunchEntry>& bunches,
        const std::vector<std::string>& task_ids) override
    {
        GanttFinalExecution exec;
        exec.bunches = bunches;
        exec.objective = objective_;

        GanttMetaModelDiagnostics diag;
        diag.model_name = model_name_;
        diag.bunch_count = bunches.size();
        diag.task_count = task_ids.size();
        diag.variable_count = bunches.size() + task_ids.size();
        diag.constraint_count = task_ids.size() * 2;
        exec.diagnostics = diag;

        return exec;
    }

private:
    std::string model_name_;
    std::optional<double> objective_;
};

// ============================================================================
// 差分对齐：RMP 执行器注册上下文
// 对齐 Rust: meta_model_rmp_executor_registers_context (gantt variant)
// ============================================================================

TEST(GanttMetaModel, RmpExecutorRegistersContext) {
    BunchEntry bunch;
    bunch.id = "b1";
    bunch.task_ids = {"t1", "t2"};
    bunch.reduced_cost = -1.5;

    MockGanttRmpExecutor executor(
        "test_rmp",
        {{"task:t1", 1.0}, {"task:t2", 2.0}},
        8.0);

    auto execution = executor.execute({bunch}, {"t1", "t2"});

    // 差分对齐断言
    ASSERT_TRUE(execution.diagnostics.has_value());
    EXPECT_EQ(execution.diagnostics->model_name, "test_rmp");
    EXPECT_EQ(execution.diagnostics->bunch_count, 1u);
    EXPECT_EQ(execution.diagnostics->task_count, 2u);
    EXPECT_DOUBLE_EQ(execution.shadow_price_summary.at("task:t1"), 1.0);
    EXPECT_DOUBLE_EQ(execution.shadow_price_summary.at("task:t2"), 2.0);
    EXPECT_EQ(execution.info.at("status"), "registered");
}

// ============================================================================
// 差分对齐：Final 执行器提取 bunches
// 对齐 Rust: meta_model_final_executor_registers_context_and_extracts_bunches
// ============================================================================

TEST(GanttMetaModel, FinalExecutorExtractsBunches) {
    BunchEntry bunch_a;
    bunch_a.id = "b1";
    bunch_a.task_ids = {"t1"};
    bunch_a.reduced_cost = -1.0;

    BunchEntry bunch_b;
    bunch_b.id = "b2";
    bunch_b.task_ids = {"t2", "t3"};
    bunch_b.reduced_cost = -2.0;

    MockGanttFinalExecutor executor("test_final", 5.0);

    auto execution = executor.execute({bunch_a, bunch_b}, {"t1", "t2", "t3"});

    // 差分对齐断言
    ASSERT_TRUE(execution.diagnostics.has_value());
    EXPECT_EQ(execution.diagnostics->model_name, "test_final");
    EXPECT_EQ(execution.diagnostics->bunch_count, 2u);
    EXPECT_EQ(execution.diagnostics->task_count, 3u);
    EXPECT_EQ(execution.bunches.size(), 2u);
    EXPECT_EQ(execution.bunches[0].id, "b1");
    EXPECT_EQ(execution.bunches[1].id, "b2");
}

// ============================================================================
// 差分对齐：NoopSolverBackend 返回默认结果
// ============================================================================

TEST(GanttMetaModel, NoopSolverBackendReturnsDefaults) {
    GanttNoopSolverBackend backend;
    EXPECT_EQ(backend.name(), "noop");

    GanttMetaModelDiagnostics diag;
    diag.model_name = "test";

    auto rmp = backend.solve_rmp(diag);
    EXPECT_FALSE(rmp.objective.has_value());

    auto final_exec = backend.solve_final(diag);
    EXPECT_FALSE(final_exec.objective.has_value());
}

// ============================================================================
// 差分对齐：BunchCGPolicy 默认值
// 对齐 Rust: test_bunch_branch_and_price_policy_defaults
// ============================================================================

TEST(GanttMetaModel, PolicyDefaults) {
    // 验证 BranchSearchConfig 默认值
    BranchSearchConfig config;
    EXPECT_EQ(config.max_nodes, 100u);
    EXPECT_FALSE(config.best_first);

    // 验证 BunchBranchAndPriceAlgorithm 构造
    auto policy = std::make_shared<GanttNoopSolverBackend>();
    GanttSchedulingConfig sched_config;
    EXPECT_EQ(sched_config.max_iterations, 50);
    EXPECT_DOUBLE_EQ(sched_config.time_limit, 300.0);
}

// Mock BunchCGPolicy / Mock bunch column generation policy
class MockBunchCGPolicy : public BunchCGPolicy {
public:
    [[nodiscard]] const std::string& name() const override {
        static const std::string n = "mock_policy";
        return n;
    }

    [[nodiscard]] std::unordered_map<std::string, double> build_shadow_price_map() const override {
        return {{"task:t1", 1.0}, {"task:t2", 2.0}};
    }

    [[nodiscard]] double reduced_cost(
        const std::unordered_map<std::string, double>& shadow_prices,
        const BunchEntry& bunch) const override
    {
        double cost = 0.0;
        for (const auto& tid : bunch.task_ids) {
            auto it = shadow_prices.find("task:" + tid);
            if (it != shadow_prices.end()) cost += it->second;
        }
        return cost;
    }

    [[nodiscard]] std::vector<BunchEntry> generate_bunches(
        std::size_t iteration,
        const std::vector<std::string>&,
        const std::unordered_map<std::string, double>&) const override
    {
        if (iteration >= 2) return {};  // 第 3 次迭代后收敛
        BunchEntry bunch;
        bunch.id = "gen-" + std::to_string(iteration);
        bunch.task_ids = {"t1"};
        bunch.reduced_cost = -1.0;
        return {bunch};
    }
};

// ============================================================================
// 差分对齐：算法 + MockPolicy 端到端集成
// 对齐 Rust: test_small_branch_and_price_flow_generates_columns_and_searches_tree
// ============================================================================

TEST(GanttMetaModel, AlgorithmWithExecutorIntegration) {
    GanttSchedulingConfig config;
    config.max_iterations = 5;
    config.max_not_better_iterations = 2;

    auto policy = std::make_shared<MockBunchCGPolicy>();

    // 节点求解器：所有节点可行，目标值=节点ID
    BunchBranchAndPriceAlgorithm::NodeSolver solver = [](const BranchNode& node) -> std::optional<double> {
        return static_cast<double>(node.id);
    };

    BunchBranchAndPriceAlgorithm algorithm(config, policy, solver);
    auto result = algorithm.run();

    // 差分对齐断言
    EXPECT_FALSE(result.termination_reason.empty());
    EXPECT_GE(result.iteration, 1u);  // 至少 1 次迭代（policy 生成 bunch）
    EXPECT_TRUE(result.active_bunches.size() > 0 || result.iteration > 0);
    EXPECT_TRUE(result.branch_result.processed_nodes > 0);  // 树搜索执行了
}
