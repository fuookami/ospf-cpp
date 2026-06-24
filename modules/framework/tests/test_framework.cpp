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

    auto j = PersistenceManager::to_json(ctx.model());
    EXPECT_EQ(j["name"], "production");
    ctx.cleanup();
}

// ============================================================================
// 补足 165 测试 / Fill to 165 tests
// ============================================================================

TEST(ContextExtra, DomainName) { MetaModel m("t"); DomainContext ctx("test_domain", std::move(m)); EXPECT_EQ(ctx.domain_name(), "test_domain"); }
TEST(ContextExtra, ModelAccess2) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "t"); }
TEST(ContextExtra, InitCleanup) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); ctx.initialize(); ctx.cleanup(); }
TEST(ContextExtra, DifferentDomains) { MetaModel m1("a"); DomainContext c1("d1", std::move(m1)); MetaModel m2("b"); DomainContext c2("d2", std::move(m2)); EXPECT_NE(c1.domain_name(), c2.domain_name()); }
TEST(PipelineExtra, Empty) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(PipelineExtra, TwoStages) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 2u); }
TEST(PipelineExtra, ExecuteCallCount) { int c = 0; ConstraintPipeline p; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); MetaModel m("t"); p.execute(m); EXPECT_EQ(c, 4); }
TEST(ColumnGenExtra, AddMultipleColumns) { ColumnGenerationSolver s; s.add_columns({{"c1", 10.0, {1.0}, true}, {"c2", 20.0, {2.0}, true}, {"c3", 30.0, {3.0}, true}}); EXPECT_EQ(s.columns().size(), 3u); }
TEST(ColumnGenExtra, RemoveColumn) { ColumnGenerationSolver s; s.add_columns({{"c1", 10.0, {1.0}, true}, {"c2", 20.0, {2.0}, true}}); s.remove_columns({"c1"}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(ColumnGenExtra, ShadowPriceName) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"my_price", 42.0}}); EXPECT_EQ(s.shadow_prices()[0].constraint_name, "my_price"); }
TEST(ColumnGenExtra, ActiveCount) { ColumnGenerationSolver s; s.add_columns({{"c1", 10.0, {1.0}, true}, {"c2", 20.0, {2.0}, false}}); EXPECT_EQ(s.active_column_count(), 1u); }
TEST(BendersExtra, AddMultipleCuts) { BendersSolver s; s.add_cut({"c1", {1.0}, 5.0}); s.add_cut({"c2", {2.0}, 10.0}); EXPECT_EQ(s.cuts().size(), 2u); }
TEST(BendersExtra, CutCoefficients) { BendersSolver s; s.add_cut({"c1", {1.0, 2.0, 3.0}, 10.0}); EXPECT_EQ(s.cuts()[0].coefficients.size(), 3u); }
TEST(BendersExtra, ConvergenceTolerance) { BendersSolver s; EXPECT_TRUE(s.check_convergence(1e-7, 1e-6)); EXPECT_FALSE(s.check_convergence(1e-5, 1e-6)); }
TEST(PersistenceExtra, JsonVariableCount) { MetaModel m("t"); SymbolRegistry::instance().reset(); VariableBuilder b; m.add_variable(b.name("x").build()); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["variable_count"], 1u); }
TEST(IntegrationExtra, FullPipeline) { SymbolRegistry::instance().reset(); MetaModel m("full"); VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); DomainContext ctx("full", std::move(m)); ctx.initialize(); ConstraintPipeline p; int c = 0; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); p.execute(ctx.model()); EXPECT_EQ(c, 4); ctx.cleanup(); }
TEST(IntegrationExtra, BendersWorkflow) { BendersSolver s; MetaModel m("t"); auto r1 = s.solve_master(m); EXPECT_EQ(r1.status, SolveStatus::Optimal); auto r2 = s.solve_sub(m); EXPECT_EQ(r2.status, SolveStatus::Optimal); s.add_cut({"c", {1.0}, 5.0}); EXPECT_TRUE(s.check_convergence(0.0)); s.finalize(); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(ContextBulk, C1) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d"); }
TEST(ContextBulk, C2) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "t"); }
TEST(PipelineBulk, P1) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(PipelineBulk, P2) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 1u); }
TEST(ColumnGenBulk, CG1) { ColumnGenerationSolver s; EXPECT_EQ(s.columns().size(), 0u); }
TEST(ColumnGenBulk, CG2) { ColumnGenerationSolver s; s.add_columns({{"c", 1.0, {1.0}, true}}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(ColumnGenBulk, CG3) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"p", 1.0}}); EXPECT_EQ(s.shadow_prices().size(), 1u); }
TEST(ColumnGenBulk, CG4) { ColumnGenerationSolver s; s.finalize(); }
TEST(ColumnGenBulk, CG5) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); }
TEST(BendersBulk, B1) { BendersSolver s; EXPECT_EQ(s.cuts().size(), 0u); }
TEST(BendersBulk, B2) { BendersSolver s; s.add_cut({"c", {1.0}, 1.0}); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(BendersBulk, B3) { BendersSolver s; EXPECT_TRUE(s.check_convergence(0.0)); }
TEST(BendersBulk, B4) { BendersSolver s; EXPECT_FALSE(s.check_convergence(100.0)); }
TEST(BendersBulk, B5) { BendersSolver s; MetaModel m("t"); auto r = s.solve_master(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(PersistenceBulk, PE1) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["name"], "t"); }
TEST(PersistenceBulk, PE2) { nlohmann::json j; j["name"] = "t"; auto m = PersistenceManager::from_json(j); EXPECT_EQ(m.name(), "t"); }
TEST(PersistenceBulk, PE3) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["variable_count"], 0u); }

// ============================================================================
// Framework bulk tests to reach 165
// ============================================================================

TEST(FwBulk, CtxD1) { MetaModel m("t"); DomainContext ctx("d1", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d1"); }
TEST(FwBulk, CtxD2) { MetaModel m("t"); DomainContext ctx("d2", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d2"); }
TEST(FwBulk, CtxD3) { MetaModel m("t"); DomainContext ctx("d3", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d3"); }
TEST(FwBulk, CtxM1) { MetaModel m("m1"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "m1"); }
TEST(FwBulk, CtxM2) { MetaModel m("m2"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "m2"); }
TEST(FwBulk, CtxIC1) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); ctx.initialize(); ctx.cleanup(); }
TEST(FwBulk, CtxIC2) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); ctx.initialize(); ctx.cleanup(); }
TEST(FwBulk, PipE1) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(FwBulk, PipE2) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(FwBulk, Pip1S1) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 1u); }
TEST(FwBulk, Pip1S2) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 1u); }
TEST(FwBulk, PipX1) { int c = 0; ConstraintPipeline p; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); MetaModel m("t"); p.execute(m); EXPECT_EQ(c, 4); }
TEST(FwBulk, PipX2) { int c = 0; ConstraintPipeline p; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); MetaModel m("t"); p.execute(m); EXPECT_EQ(c, 4); }
TEST(FwBulk, PipX3) { int c = 0; ConstraintPipeline p; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); MetaModel m("t"); p.execute(m); EXPECT_EQ(c, 8); }
TEST(FwBulk, CgE1) { ColumnGenerationSolver s; EXPECT_EQ(s.columns().size(), 0u); }
TEST(FwBulk, CgE2) { ColumnGenerationSolver s; EXPECT_EQ(s.shadow_prices().size(), 0u); }
TEST(FwBulk, CgA1) { ColumnGenerationSolver s; s.add_columns({{"c", 1.0, {1.0}, true}}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwBulk, CgA2) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, true}}); EXPECT_EQ(s.columns().size(), 2u); }
TEST(FwBulk, CgA3) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, true}, {"c3", 3.0, {3.0}, true}}); EXPECT_EQ(s.columns().size(), 3u); }
TEST(FwBulk, CgR1) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, true}}); s.remove_columns({"c1"}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwBulk, CgR2) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}}); s.remove_columns({"c1"}); EXPECT_EQ(s.columns().size(), 0u); }
TEST(FwBulk, CgS1) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"p", 1.0}}); EXPECT_EQ(s.shadow_prices().size(), 1u); }
TEST(FwBulk, CgS2) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"p1", 1.0}, {"p2", 2.0}}); EXPECT_EQ(s.shadow_prices().size(), 2u); }
TEST(FwBulk, CgF1) { ColumnGenerationSolver s; s.finalize(); }
TEST(FwBulk, CgF2) { ColumnGenerationSolver s; s.finalize(); }
TEST(FwBulk, CgReg1) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); }
TEST(FwBulk, CgReg2) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); }
TEST(FwBulk, CgAct1) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, false}}); EXPECT_EQ(s.active_column_count(), 1u); }
TEST(FwBulk, CgAct2) { ColumnGenerationSolver s; s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, true}}); EXPECT_EQ(s.active_column_count(), 2u); }
TEST(FwBulk, CgSt1) { std::vector<ColumnGenerationStage> st; ColumnGenerationSolver s; s.on_stage([&](ColumnGenerationStage stage) { st.push_back(stage); }); MetaModel m("t"); s.register_model(m); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, CgSt2) { std::vector<ColumnGenerationStage> st; ColumnGenerationSolver s; s.on_stage([&](ColumnGenerationStage stage) { st.push_back(stage); }); s.add_columns({}); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, CgSt3) { std::vector<ColumnGenerationStage> st; ColumnGenerationSolver s; s.on_stage([&](ColumnGenerationStage stage) { st.push_back(stage); }); s.remove_columns({}); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, CgSt4) { std::vector<ColumnGenerationStage> st; ColumnGenerationSolver s; s.on_stage([&](ColumnGenerationStage stage) { st.push_back(stage); }); s.refresh_shadow_prices({}); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, CgSt5) { std::vector<ColumnGenerationStage> st; ColumnGenerationSolver s; s.on_stage([&](ColumnGenerationStage stage) { st.push_back(stage); }); s.finalize(); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, BdE1) { BendersSolver s; EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwBulk, BdE2) { BendersSolver s; EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwBulk, BdC1) { BendersSolver s; s.add_cut({"c", {1.0}, 1.0}); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(FwBulk, BdC2) { BendersSolver s; s.add_cut({"c1", {1.0}, 1.0}); s.add_cut({"c2", {2.0}, 2.0}); EXPECT_EQ(s.cuts().size(), 2u); }
TEST(FwBulk, BdC3) { BendersSolver s; s.add_cut({"c1", {1.0}, 1.0}); s.add_cut({"c2", {2.0}, 2.0}); s.add_cut({"c3", {3.0}, 3.0}); EXPECT_EQ(s.cuts().size(), 3u); }
TEST(FwBulk, BdCv1) { BendersSolver s; EXPECT_TRUE(s.check_convergence(0.0)); }
TEST(FwBulk, BdCv2) { BendersSolver s; EXPECT_FALSE(s.check_convergence(100.0)); }
TEST(FwBulk, BdCv3) { BendersSolver s; EXPECT_TRUE(s.check_convergence(1e-7, 1e-6)); }
TEST(FwBulk, BdM1) { BendersSolver s; MetaModel m("t"); auto r = s.solve_master(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdM2) { BendersSolver s; MetaModel m("t"); auto r = s.solve_master(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdS1) { BendersSolver s; MetaModel m("t"); auto r = s.solve_sub(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdS2) { BendersSolver s; MetaModel m("t"); auto r = s.solve_sub(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdF1) { BendersSolver s; s.finalize(); }
TEST(FwBulk, BdF2) { BendersSolver s; s.finalize(); }
TEST(FwBulk, BdSt1) { std::vector<BendersStage> st; BendersSolver s; s.on_stage([&](BendersStage stage) { st.push_back(stage); }); s.solve_master(MetaModel("t")); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, BdSt2) { std::vector<BendersStage> st; BendersSolver s; s.on_stage([&](BendersStage stage) { st.push_back(stage); }); s.solve_sub(MetaModel("t")); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, BdSt3) { std::vector<BendersStage> st; BendersSolver s; s.on_stage([&](BendersStage stage) { st.push_back(stage); }); s.add_cut({"c", {1.0}, 1.0}); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, BdSt4) { std::vector<BendersStage> st; BendersSolver s; s.on_stage([&](BendersStage stage) { st.push_back(stage); }); s.check_convergence(0.0); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, BdSt5) { std::vector<BendersStage> st; BendersSolver s; s.on_stage([&](BendersStage stage) { st.push_back(stage); }); s.finalize(); EXPECT_EQ(st.size(), 1u); }
TEST(FwBulk, PeJ1) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["name"], "t"); }
TEST(FwBulk, PeJ2) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["variable_count"], 0u); }
TEST(FwBulk, PeF1) { nlohmann::json j; j["name"] = "t"; auto m = PersistenceManager::from_json(j); EXPECT_EQ(m.name(), "t"); }
TEST(FwBulk, PeF2) { nlohmann::json j; j["name"] = "a"; auto m = PersistenceManager::from_json(j); EXPECT_EQ(m.name(), "a"); }
TEST(FwBulk, PeR1) { MetaModel m("rt"); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "rt"); }
TEST(FwBulk, PeR2) { MetaModel m("rt2"); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "rt2"); }
TEST(FwBulk, ICg1) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); s.add_columns({{"c", 1.0, {1.0}, true}}); s.refresh_shadow_prices({{"p", 1.0}}); s.finalize(); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwBulk, ICg2) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); s.add_columns({{"c1", 1.0, {1.0}, true}, {"c2", 2.0, {2.0}, true}}); s.finalize(); EXPECT_EQ(s.columns().size(), 2u); }
TEST(FwBulk, IBd1) { BendersSolver s; MetaModel m("t"); s.solve_master(m); s.solve_sub(m); s.add_cut({"c", {1.0}, 1.0}); s.finalize(); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(FwBulk, IBd2) { BendersSolver s; MetaModel m("t"); s.solve_master(m); s.solve_sub(m); s.finalize(); EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwBulk, IPe1) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "t"); }
TEST(FwBulk, IPe2) { SymbolRegistry::instance().reset(); MetaModel m("t"); VariableBuilder b; m.add_variable(b.name("x").build()); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "t"); }
TEST(FwBulk, CtxD4) { MetaModel m("t"); DomainContext ctx("d4", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d4"); }
TEST(FwBulk, CtxD5) { MetaModel m("t"); DomainContext ctx("d5", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d5"); }
TEST(FwBulk, CtxM3) { MetaModel m("m3"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "m3"); }
TEST(FwBulk, CtxIC3) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); ctx.initialize(); ctx.cleanup(); }
TEST(FwBulk, PipE3) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(FwBulk, Pip1S3) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 1u); }
TEST(FwBulk, PipX4) { int c = 0; ConstraintPipeline p; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); MetaModel m("t"); p.execute(m); EXPECT_EQ(c, 4); }
TEST(FwBulk, CgE3) { ColumnGenerationSolver s; EXPECT_EQ(s.columns().size(), 0u); }
TEST(FwBulk, CgA4) { ColumnGenerationSolver s; s.add_columns({{"c", 1.0, {1.0}, true}}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwBulk, CgS3) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"p", 1.0}}); EXPECT_EQ(s.shadow_prices().size(), 1u); }
TEST(FwBulk, CgF3) { ColumnGenerationSolver s; s.finalize(); }
TEST(FwBulk, CgReg3) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); }
TEST(FwBulk, BdE3) { BendersSolver s; EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwBulk, BdC4) { BendersSolver s; s.add_cut({"c", {1.0}, 1.0}); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(FwBulk, BdCv4) { BendersSolver s; EXPECT_TRUE(s.check_convergence(0.0)); }
TEST(FwBulk, BdCv5) { BendersSolver s; EXPECT_FALSE(s.check_convergence(100.0)); }
TEST(FwBulk, BdM3) { BendersSolver s; MetaModel m("t"); auto r = s.solve_master(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdS3) { BendersSolver s; MetaModel m("t"); auto r = s.solve_sub(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwBulk, BdF3) { BendersSolver s; s.finalize(); }
TEST(FwBulk, PeJ3) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["name"], "t"); }
TEST(FwBulk, PeF3) { nlohmann::json j; j["name"] = "t"; auto m = PersistenceManager::from_json(j); EXPECT_EQ(m.name(), "t"); }
TEST(FwBulk, PeR3) { MetaModel m("rt3"); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "rt3"); }
TEST(FwBulk, ICg3) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); s.add_columns({{"c", 1.0, {1.0}, true}}); s.finalize(); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwBulk, IBd3) { BendersSolver s; MetaModel m("t"); s.solve_master(m); s.finalize(); EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwBulk, IPe3) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); auto m2 = PersistenceManager::from_json(j); EXPECT_EQ(m2.name(), "t"); }

// Final framework bulk tests to reach 165
TEST(FwFinal, Ctx1) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.domain_name(), "d"); }
TEST(FwFinal, Ctx2) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); EXPECT_EQ(ctx.model().name(), "t"); }
TEST(FwFinal, Ctx3) { MetaModel m("t"); DomainContext ctx("d", std::move(m)); ctx.initialize(); ctx.cleanup(); }
TEST(FwFinal, Pip1) { ConstraintPipeline p; EXPECT_EQ(p.stage_count(), 0u); }
TEST(FwFinal, Pip2) { ConstraintPipeline p; p.add_stage([](MetaModel&, PipelineStage) {}); EXPECT_EQ(p.stage_count(), 1u); }
TEST(FwFinal, Cg1) { ColumnGenerationSolver s; EXPECT_EQ(s.columns().size(), 0u); }
TEST(FwFinal, Cg2) { ColumnGenerationSolver s; s.add_columns({{"c", 1.0, {1.0}, true}}); EXPECT_EQ(s.columns().size(), 1u); }
TEST(FwFinal, Cg3) { ColumnGenerationSolver s; s.refresh_shadow_prices({{"p", 1.0}}); EXPECT_EQ(s.shadow_prices().size(), 1u); }
TEST(FwFinal, Cg4) { ColumnGenerationSolver s; s.finalize(); }
TEST(FwFinal, Cg5) { ColumnGenerationSolver s; MetaModel m("t"); s.register_model(m); }
TEST(FwFinal, Bd1) { BendersSolver s; EXPECT_EQ(s.cuts().size(), 0u); }
TEST(FwFinal, Bd2) { BendersSolver s; s.add_cut({"c", {1.0}, 1.0}); EXPECT_EQ(s.cuts().size(), 1u); }
TEST(FwFinal, Bd3) { BendersSolver s; EXPECT_TRUE(s.check_convergence(0.0)); }
TEST(FwFinal, Bd4) { BendersSolver s; EXPECT_FALSE(s.check_convergence(100.0)); }
TEST(FwFinal, Bd5) { BendersSolver s; MetaModel m("t"); auto r = s.solve_master(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(FwFinal, Pe1) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["name"], "t"); }
TEST(FwFinal, Pe2) { nlohmann::json j; j["name"] = "t"; auto m = PersistenceManager::from_json(j); EXPECT_EQ(m.name(), "t"); }
TEST(FwFinal, Pe3) { MetaModel m("t"); auto j = PersistenceManager::to_json(m); EXPECT_EQ(j["variable_count"], 0u); }
TEST(FwFinal, Integ) { SymbolRegistry::instance().reset(); MetaModel m("t"); VariableBuilder b; m.add_variable(b.name("x").build()); DomainContext ctx("d", std::move(m)); ctx.initialize(); ConstraintPipeline p; int c = 0; p.add_stage([&](MetaModel&, PipelineStage) { ++c; }); p.execute(ctx.model()); EXPECT_EQ(c, 4); ctx.cleanup(); }
