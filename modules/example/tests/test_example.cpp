/// ospf_example 测试
/// 覆盖 example 模块基本功能 + demo 验证

#include <gtest/gtest.h>
#include <ospf/example/core_shortcuts.hpp>
#include <ospf/example/common.hpp>
#include <ospf/example/core/demo1.hpp>
#include <ospf/example/core/demo2.hpp>
#include <ospf/example/core/demo3.hpp>
#include <ospf/example/framework/demo1.hpp>
#include <ospf/example/framework/demo4.hpp>

using namespace ospf::example;
namespace sym = ospf::math::symbol;

TEST(Example, CommonResult) {
    ExampleResult r{"test", 42.0, true};
    EXPECT_EQ(r.name, "test");
    EXPECT_DOUBLE_EQ(r.objective, 42.0);
    EXPECT_TRUE(r.feasible);
}

TEST(Example, CommonResultDefault) {
    ExampleResult r;
    EXPECT_TRUE(r.name.empty());
    EXPECT_DOUBLE_EQ(r.objective, 0.0);
    EXPECT_FALSE(r.feasible);
}

TEST(Example, CoreShortcuts) {
    // 验证 core 模块可用
    SymbolRegistry::instance().reset();
    VariableBuilder b;
    auto x = b.name("x").lower(0).upper(10).build();
    EXPECT_EQ(x.name(), "x");
}

TEST(Example, MetaModelUsage) {
    SymbolRegistry::instance().reset();
    MetaModel m("example");
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).upper(10).build());
    m.set_objective("obj", ObjectiveSense::Maximize, 
        sym::Expression(sym::Variable("x", 1)));
    EXPECT_TRUE(m.is_valid());
}

TEST(Example, SolverUsage) {
    SymbolRegistry::instance().reset();
    MetaModel m("example");
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).upper(10).build());
    m.set_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Variable("x", 1)));

    StubSolver solver;
    auto result = solver.solve(m);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}

// ============================================================================
// Core Demo 测试 / Core Demo tests
// ============================================================================

TEST(CoreDemo, Demo1Portfolio) {
    ospf::example::core::demo1();
    // demo1 执行成功即通过
    EXPECT_TRUE(true);
}

TEST(CoreDemo, Demo2ProductMix) {
    ospf::example::core::demo2();
    EXPECT_TRUE(true);
}

TEST(CoreDemo, Demo3ResourceAllocation) {
    ospf::example::core::demo3();
    EXPECT_TRUE(true);
}

// ============================================================================
// Framework Demo 测试 / Framework Demo tests
// ============================================================================

TEST(FrameworkDemo, Demo1Bandwidth) {
    ospf::example::framework::demo1_bandwidth();
    EXPECT_TRUE(true);
}

TEST(FrameworkDemo, Demo4FlightScheduling) {
    ospf::example::framework::demo4_flight();
    EXPECT_TRUE(true);
}

// ============================================================================
// 集成测试 / Integration tests
// ============================================================================

TEST(ExampleIntegration, PortfolioOptimization) {
    using namespace ospf::core;
    using namespace ospf::math::symbol;

    SymbolRegistry::instance().reset();
    MetaModel model("portfolio_test");

    VariableBuilder b;
    model.add_variable(b.name("x1").type(VariableType::Binary).build());
    model.add_variable(b.name("x2").type(VariableType::Binary).build());
    model.add_variable(b.name("x3").type(VariableType::Binary).build());

    // max 10*x1 + 8*x2 + 12*x3
    model.set_objective("profit", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(10.0, sym::Variable("x1", 0))) +
        sym::Expression(sym::Monomial(8.0, sym::Variable("x2", 1))) +
        sym::Expression(sym::Monomial(12.0, sym::Variable("x3", 2))));

    // x1 + x2 + x3 <= 2
    model.add_constraint("budget",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x1", 0)) + sym::Expression(sym::Variable("x2", 1)) + sym::Expression(sym::Variable("x3", 2)),
            sym::Expression(2.0)));

    EXPECT_TRUE(model.is_valid());
    EXPECT_EQ(model.variable_count(), 3u);
    EXPECT_EQ(model.constraint_count(), 1u);

    StubSolver solver;
    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}

TEST(ExampleIntegration, ResourceAllocationModel) {
    using namespace ospf::core;
    using namespace ospf::math::symbol;

    SymbolRegistry::instance().reset();
    MetaModel model("resource");

    VariableBuilder b;
    model.add_variable(b.name("a").lower(0).upper(100).build());
    model.add_variable(b.name("b").lower(0).upper(100).build());

    // max 5a + 3b
    model.set_objective("收益", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(5.0, sym::Variable("a", 0))) +
        sym::Expression(sym::Monomial(3.0, sym::Variable("b", 1))));

    // a + b <= 80
    model.add_constraint("资源",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("a", 0)) + sym::Expression(sym::Variable("b", 1)),
            sym::Expression(80.0)));

    EXPECT_TRUE(model.is_valid());

    StubSolver solver;
    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}

TEST(ExampleIntegration, FlightSchedulingModel) {
    using namespace ospf::core;
    using namespace ospf::math::symbol;

    SymbolRegistry::instance().reset();
    MetaModel model("flight");

    VariableBuilder b;
    model.add_variable(b.name("assign").type(VariableType::Binary).build());

    model.set_objective("cost", ObjectiveSense::Minimize,
        sym::Expression(sym::Monomial(100.0, sym::Variable("assign", 0))));

    EXPECT_TRUE(model.is_valid());

    StubSolver solver;
    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}
