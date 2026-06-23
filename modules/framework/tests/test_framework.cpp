/// ospf_framework 测试
/// 覆盖 context、pipeline、column_generation、benders、persistence。

#include <gtest/gtest.h>

#include <ospf/framework/context.hpp>
#include <ospf/framework/pipeline.hpp>
#include <ospf/framework/column_generation.hpp>
#include <ospf/framework/benders.hpp>
#include <ospf/framework/persistence.hpp>

using namespace ospf::framework;
using namespace ospf::core;

// ============================================================================
// Context 测试 / Context tests
// ============================================================================

TEST(Context, DomainContextCreation) {
    MetaModel model("test");
    DomainContext ctx("BPP3D", std::move(model));
    EXPECT_EQ(ctx.domain_name(), "BPP3D");
}

TEST(Context, ModelAccess) {
    MetaModel model("test");
    VariableBuilder builder;
    SymbolRegistry::instance().reset();
    auto x = builder.name("x").build();
    model.add_variable(std::move(x));

    DomainContext ctx("test", std::move(model));
    EXPECT_EQ(ctx.model().variable_count(), 1u);
}

TEST(Context, InitializeCleanup) {
    MetaModel model("test");
    DomainContext ctx("test", std::move(model));
    ctx.initialize();
    ctx.cleanup();
}

// ============================================================================
// Pipeline 测试 / Pipeline tests
// ============================================================================

TEST(Pipeline, Creation) {
    ConstraintPipeline pipeline;
    EXPECT_EQ(pipeline.stage_count(), 0u);
}

TEST(Pipeline, AddStage) {
    ConstraintPipeline pipeline;
    pipeline.add_stage([](MetaModel&, PipelineStage) {});
    EXPECT_EQ(pipeline.stage_count(), 1u);
}

TEST(Pipeline, Execute) {
    ConstraintPipeline pipeline;
    int call_count = 0;

    pipeline.add_stage([&](MetaModel& /*model*/, PipelineStage /*stage*/) {
        ++call_count;
    });

    MetaModel model("test");
    pipeline.execute(model);

    // 每个阶段被调用 4 次（Registration, Validation, Transformation, Finalization）
    EXPECT_EQ(call_count, 4);
}

// ============================================================================
// ColumnGeneration 测试 / Column generation tests
// ============================================================================

TEST(ColumnGeneration, Creation) {
    ColumnGenerationSolver solver;
    EXPECT_EQ(solver.columns().size(), 0u);
    EXPECT_EQ(solver.shadow_prices().size(), 0u);
}

TEST(ColumnGeneration, RegisterModel) {
    ColumnGenerationSolver solver;
    MetaModel model("test");
    solver.register_model(model);
}

TEST(ColumnGeneration, AddColumns) {
    ColumnGenerationSolver solver;
    std::vector<ColumnInfo> columns = {
        {"col1", 10.0, {1.0, 2.0}, true},
        {"col2", 20.0, {3.0, 4.0}, true}
    };
    solver.add_columns(std::move(columns));
    EXPECT_EQ(solver.columns().size(), 2u);
    EXPECT_EQ(solver.active_column_count(), 2u);
}

TEST(ColumnGeneration, RemoveColumns) {
    ColumnGenerationSolver solver;
    std::vector<ColumnInfo> columns = {
        {"col1", 10.0, {1.0, 2.0}, true},
        {"col2", 20.0, {3.0, 4.0}, true}
    };
    solver.add_columns(std::move(columns));
    solver.remove_columns({"col1"});
    EXPECT_EQ(solver.columns().size(), 1u);
    EXPECT_EQ(solver.columns()[0].name, "col2");
}

TEST(ColumnGeneration, ShadowPrices) {
    ColumnGenerationSolver solver;
    std::vector<ShadowPrice> prices = {
        {"c1", 5.0},
        {"c2", 10.0}
    };
    solver.refresh_shadow_prices(std::move(prices));
    EXPECT_EQ(solver.shadow_prices().size(), 2u);
    EXPECT_DOUBLE_EQ(solver.shadow_prices()[0].price, 5.0);
}

TEST(ColumnGeneration, StageCallback) {
    ColumnGenerationSolver solver;
    std::vector<ColumnGenerationStage> stages;

    solver.on_stage([&](ColumnGenerationStage stage) {
        stages.push_back(stage);
    });

    MetaModel model("test");
    solver.register_model(model);
    solver.add_columns({{"col1", 10.0, {1.0}, true}});
    solver.remove_columns({"col1"});
    solver.refresh_shadow_prices({{"c1", 5.0}});
    solver.finalize();

    EXPECT_EQ(stages.size(), 5u);
    EXPECT_EQ(stages[0], ColumnGenerationStage::RegisterModel);
    EXPECT_EQ(stages[1], ColumnGenerationStage::AddColumns);
    EXPECT_EQ(stages[2], ColumnGenerationStage::RemoveColumns);
    EXPECT_EQ(stages[3], ColumnGenerationStage::RefreshShadowPrice);
    EXPECT_EQ(stages[4], ColumnGenerationStage::Finalize);
}

// ============================================================================
// Benders 测试 / Benders tests
// ============================================================================

TEST(Benders, Creation) {
    BendersSolver solver;
    EXPECT_EQ(solver.cuts().size(), 0u);
}

TEST(Benders, SolveMaster) {
    BendersSolver solver;
    MetaModel model("test");
    auto result = solver.solve_master(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}

TEST(Benders, SolveSub) {
    BendersSolver solver;
    MetaModel model("test");
    auto result = solver.solve_sub(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}

TEST(Benders, AddCut) {
    BendersSolver solver;
    BendersCut cut{"cut1", {1.0, 2.0}, 10.0};
    solver.add_cut(std::move(cut));
    EXPECT_EQ(solver.cuts().size(), 1u);
    EXPECT_EQ(solver.cuts()[0].name, "cut1");
}

TEST(Benders, Convergence) {
    BendersSolver solver;
    EXPECT_TRUE(solver.check_convergence(1e-10));
    EXPECT_FALSE(solver.check_convergence(1.0));
}

TEST(Benders, StageCallback) {
    BendersSolver solver;
    std::vector<BendersStage> stages;

    solver.on_stage([&](BendersStage stage) {
        stages.push_back(stage);
    });

    MetaModel model("test");
    solver.solve_master(model);
    solver.solve_sub(model);
    solver.add_cut({"cut1", {1.0}, 5.0});
    solver.check_convergence(0.0);
    solver.finalize();

    EXPECT_EQ(stages.size(), 5u);
}

// ============================================================================
// Persistence 测试 / Persistence tests
// ============================================================================

TEST(Persistence, ToJson) {
    MetaModel model("test");
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto x = builder.name("x").lower(0).upper(10).build();
    model.add_variable(std::move(x));

    auto j = PersistenceManager::to_json(model);
    EXPECT_EQ(j["name"], "test");
    EXPECT_EQ(j["variable_count"], 1u);
}

TEST(Persistence, FromJson) {
    nlohmann::json j;
    j["name"] = "loaded";
    auto model = PersistenceManager::from_json(j);
    EXPECT_EQ(model.name(), "loaded");
}

TEST(Persistence, RoundTrip) {
    MetaModel model("round_trip");
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto x = builder.name("x").lower(0).upper(10).build();
    model.add_variable(std::move(x));

    auto j = PersistenceManager::to_json(model);
    auto loaded = PersistenceManager::from_json(j);
    EXPECT_EQ(loaded.name(), "round_trip");
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, FrameworkWorkflow) {
    SymbolRegistry::instance().reset();

    // 创建模型
    MetaModel model("production");

    // 注册变量
    VariableBuilder builder;
    auto x1 = builder.name("x1").lower(0).upper(100).build();
    auto x2 = builder.name("x2").lower(0).upper(80).build();
    model.add_variable(std::move(x1));
    model.add_variable(std::move(x2));

    // 创建领域上下文
    DomainContext ctx("production", std::move(model));
    ctx.initialize();

    // 创建约束管线
    ConstraintPipeline pipeline;
    pipeline.add_stage([](MetaModel& /*m*/, PipelineStage stage) {
        if (stage == PipelineStage::Registration) {
            // 注册额外约束
        }
    });
    pipeline.execute(ctx.model());

    // 创建列生成求解器
    ColumnGenerationSolver cg_solver;
    cg_solver.register_model(ctx.model());
    cg_solver.add_columns({
        {"col1", 10.0, {1.0, 0.0}, true},
        {"col2", 20.0, {0.0, 1.0}, true}
    });
    cg_solver.refresh_shadow_prices({{"c1", 5.0}});
    cg_solver.finalize();

    // 验证
    EXPECT_EQ(cg_solver.columns().size(), 2u);
    EXPECT_EQ(cg_solver.shadow_prices().size(), 1u);

    // 持久化
    auto j = PersistenceManager::to_json(ctx.model());
    EXPECT_EQ(j["name"], "production");

    ctx.cleanup();
}
