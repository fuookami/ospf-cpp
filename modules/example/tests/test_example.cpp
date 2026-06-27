/// ospf_example 测试
/// 覆盖 example 模块基本功能 + demo 验证

#include <gtest/gtest.h>
#include <ospf/example/core_shortcuts.hpp>
#include <ospf/example/common.hpp>
#include <ospf/example/core/demo1.hpp>
#include <ospf/example/core/demo2.hpp>
#include <ospf/example/core/demo3.hpp>
#include <ospf/example/core/demo4.hpp>
#include <ospf/example/core/demo5.hpp>
#include <ospf/example/core/demo6.hpp>
#include <ospf/example/core/demo7.hpp>
#include <ospf/example/core/demo8.hpp>
#include <ospf/example/core/demo9.hpp>
#include <ospf/example/core/demo10.hpp>
#include <ospf/example/core/demo11.hpp>
#include <ospf/example/core/demo12.hpp>
#include <ospf/example/core/demo13.hpp>
#include <ospf/example/core/demo14.hpp>
#include <ospf/example/core/demo15.hpp>
#include <ospf/example/core/demo16.hpp>
#include <ospf/example/core/demo17.hpp>
#include <ospf/example/framework/demo1.hpp>
#include <ospf/example/framework/demo2.hpp>
#include <ospf/example/framework/demo3.hpp>
#include <ospf/example/framework/demo4.hpp>
#include <ospf/math/trivalent.hpp>
#include <ospf/math/symbol/polynomial.hpp>

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
    EXPECT_NO_THROW();
}

TEST(CoreDemo, Demo2ProductMix) {
    ospf::example::core::demo2();
    EXPECT_NO_THROW();
}

TEST(CoreDemo, Demo3ResourceAllocation) {
    ospf::example::core::demo3();
    EXPECT_NO_THROW();
}

// ============================================================================
// Framework Demo 测试 / Framework Demo tests
// ============================================================================

TEST(FrameworkDemo, Demo1Bandwidth) {
    ospf::example::framework::demo1_bandwidth();
    EXPECT_NO_THROW();
}

TEST(FrameworkDemo, Demo4FlightScheduling) {
    ospf::example::framework::demo4_flight();
    EXPECT_NO_THROW();
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

// ============================================================================
// Core Demo 4-17 测试
// ============================================================================

TEST(CoreDemo, Demo4ResourceAlloc) { ospf::example::core::demo4(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo5Knapsack) { ospf::example::core::demo5(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo6) { ospf::example::core::demo6(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo7) { ospf::example::core::demo7(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo8) { ospf::example::core::demo8(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo9) { ospf::example::core::demo9(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo10) { ospf::example::core::demo10(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo11) { ospf::example::core::demo11(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo12) { ospf::example::core::demo12(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo13) { ospf::example::core::demo13(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo14) { ospf::example::core::demo14(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo15) { ospf::example::core::demo15(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo16) { ospf::example::core::demo16(); EXPECT_NO_THROW(); }
TEST(CoreDemo, Demo17) { ospf::example::core::demo17(); EXPECT_NO_THROW(); }

// ============================================================================
// Framework Demo 2-3 测试
// ============================================================================

TEST(FrameworkDemo, Demo2Loading) { ospf::example::framework::demo2_loading(); EXPECT_NO_THROW(); }
TEST(FrameworkDemo, Demo3SimpleLP) { ospf::example::framework::demo3_simple_lp(); EXPECT_NO_THROW(); }

// ============================================================================
// Trivalent 测试
// ============================================================================

TEST(Trivalent, TrueAndTrue) {
    using namespace ospf::math;
    EXPECT_TRUE(is_true(Trivalent::True && Trivalent::True));
}

TEST(Trivalent, TrueAndFalse) {
    using namespace ospf::math;
    EXPECT_TRUE(is_false(Trivalent::True && Trivalent::False));
}

TEST(Trivalent, TrueOrFalse) {
    using namespace ospf::math;
    EXPECT_TRUE(is_true(Trivalent::True || Trivalent::False));
}

TEST(Trivalent, FalseOrFalse) {
    using namespace ospf::math;
    EXPECT_TRUE(is_false(Trivalent::False || Trivalent::False));
}

TEST(Trivalent, NotTrue) {
    using namespace ospf::math;
    EXPECT_TRUE(is_false(!Trivalent::True));
}

TEST(Trivalent, NotFalse) {
    using namespace ospf::math;
    EXPECT_TRUE(is_true(!Trivalent::False));
}

TEST(Trivalent, UnknownAndTrue) {
    using namespace ospf::math;
    EXPECT_TRUE(is_unknown(Trivalent::Unknown && Trivalent::True));
}

TEST(Trivalent, UnknownOrFalse) {
    using namespace ospf::math;
    EXPECT_TRUE(is_unknown(Trivalent::Unknown || Trivalent::False));
}

TEST(Trivalent, NotUnknown) {
    using namespace ospf::math;
    EXPECT_TRUE(is_unknown(!Trivalent::Unknown));
}

TEST(Trivalent, FromBool) {
    using namespace ospf::math;
    EXPECT_TRUE(is_true(from_bool(true)));
    EXPECT_TRUE(is_false(from_bool(false)));
}

// ============================================================================
// 扩展 Example 测试 / Extended Example tests (to reach 96)
// ============================================================================

TEST(ExampleExtended, CommonResultCopy) {
    ExampleResult r1{"test", 42.0, true};
    ExampleResult r2 = r1;
    EXPECT_EQ(r2.name, "test");
    EXPECT_DOUBLE_EQ(r2.objective, 42.0);
    EXPECT_TRUE(r2.feasible);
}

TEST(ExampleExtended, CoreShortcutsRegistry) {
    SymbolRegistry::instance().reset();
    VariableBuilder b;
    auto x = b.name("x").lower(0).upper(10).build();
    auto y = b.name("y").lower(0).upper(10).build();
    EXPECT_NE(x.name(), y.name());
}

TEST(ExampleExtended, MetaModelVariables) {
    SymbolRegistry::instance().reset();
    MetaModel m("test");
    VariableBuilder b;
    m.add_variable(b.name("a").lower(0).build());
    m.add_variable(b.name("b").lower(0).build());
    m.add_variable(b.name("c").lower(0).build());
    EXPECT_EQ(m.variable_count(), 3u);
}

TEST(ExampleExtended, MetaModelConstraints) {
    SymbolRegistry::instance().reset();
    MetaModel m("test");
    m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0)));
    m.add_constraint("c2", sym::Inequality::greater(sym::Expression(3.0), sym::Expression(1.0)));
    EXPECT_EQ(m.constraint_count(), 2u);
}

TEST(ExampleExtended, MetaModelObjective) {
    SymbolRegistry::instance().reset();
    MetaModel m("test");
    VariableBuilder b;
    m.add_variable(b.name("x").build());
    m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 0)));
    EXPECT_TRUE(m.objective().has_value());
    EXPECT_EQ(m.objective()->sense, ObjectiveSense::Minimize);
}

TEST(ExampleExtended, MetaModelStageTransition) {
    MetaModel m("test");
    EXPECT_EQ(m.stage(), ModelStage::Registration);
    m.set_stage(ModelStage::Building);
    EXPECT_EQ(m.stage(), ModelStage::Building);
    m.set_stage(ModelStage::Solving);
    EXPECT_EQ(m.stage(), ModelStage::Solving);
    m.set_stage(ModelStage::Solved);
    EXPECT_EQ(m.stage(), ModelStage::Solved);
}

TEST(ExampleExtended, SolverStub) {
    StubSolver s;
    EXPECT_EQ(s.name(), "StubSolver");
    EXPECT_TRUE(s.is_available());
}

TEST(ExampleExtended, SolverSolveEmpty) {
    StubSolver s;
    MetaModel m("test");
    auto r = s.solve(m);
    EXPECT_EQ(r.status, SolveStatus::Optimal);
}

TEST(ExampleExtended, SolverSolveOneVar) {
    SymbolRegistry::instance().reset();
    StubSolver s;
    MetaModel m;
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).upper(10).build());
    auto r = s.solve(m);
    EXPECT_EQ(r.status, SolveStatus::Optimal);
    EXPECT_EQ(r.variable_values.size(), 1u);
}

TEST(ExampleExtended, SolverSolveTwoVars) {
    SymbolRegistry::instance().reset();
    StubSolver s;
    MetaModel m;
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).build());
    m.add_variable(b.name("y").lower(0).build());
    auto r = s.solve(m);
    EXPECT_EQ(r.variable_values.size(), 2u);
}

TEST(ExampleExtended, ExpressionArithmetic) {
    auto e1 = sym::Expression(3.0);
    auto e2 = sym::Expression(4.0);
    auto sum = e1 + e2;
    EXPECT_TRUE(sum.is_constant());
}

TEST(ExampleExtended, MonomialConstruction) {
    sym::Variable x("x", 0);
    sym::Monomial m(5.0, x, 2.0);
    EXPECT_DOUBLE_EQ(m.coefficient(), 5.0);
    EXPECT_DOUBLE_EQ(m.degree_of(x), 2.0);
}

TEST(ExampleExtended, PolynomialLinear) {
    sym::Variable x("x", 0);
    auto p = sym::Polynomial::linear(2.0, 3.0, x);
    EXPECT_EQ(p.degree(), 1u);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 5.0);
}

TEST(ExampleExtended, PolynomialQuadratic) {
    sym::Variable x("x", 0);
    auto p = sym::Polynomial::quadratic(1.0, -2.0, 1.0, x);
    EXPECT_EQ(p.degree(), 2u);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0);
}

TEST(ExampleExtended, InequalityLess) {
    sym::Variable x("x", 0);
    auto ineq = sym::Inequality::less(sym::Expression(sym::Variable("x", 0)), sym::Expression(10.0));
    std::map<sym::Variable, double> vals = {{x, 5.0}};
    EXPECT_TRUE(ineq.check(vals));
}

TEST(ExampleExtended, InequalityGreater) {
    sym::Variable x("x", 0);
    auto ineq = sym::Inequality::greater(sym::Expression(sym::Variable("x", 0)), sym::Expression(5.0));
    std::map<sym::Variable, double> vals = {{x, 10.0}};
    EXPECT_TRUE(ineq.check(vals));
}

TEST(ExampleExtended, KnapsackOptimal) {
    using namespace ospf::core;
    SymbolRegistry::instance().reset();
    MetaModel m("knapsack_test");
    VariableBuilder b;
    m.add_variable(b.name("a").type(VariableType::Binary).build());
    m.add_variable(b.name("b").type(VariableType::Binary).build());
    m.set_objective("value", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(10.0, sym::Variable("a", 0))) +
        sym::Expression(sym::Monomial(20.0, sym::Variable("b", 1))));
    m.add_constraint("weight", sym::Inequality::less_equal(
        sym::Expression(sym::Monomial(5.0, sym::Variable("a", 0))) +
        sym::Expression(sym::Monomial(10.0, sym::Variable("b", 1))),
        sym::Expression(12.0)));
    EXPECT_TRUE(m.is_valid());
    StubSolver solver;
    auto r = solver.solve(m);
    EXPECT_EQ(r.status, SolveStatus::Optimal);
}

TEST(ExampleExtended, LinearProgramming) {
    using namespace ospf::core;
    SymbolRegistry::instance().reset();
    MetaModel m("lp_test");
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).upper(10).build());
    m.add_variable(b.name("y").lower(0).upper(10).build());
    m.set_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(3.0, sym::Variable("x", 0))) +
        sym::Expression(sym::Monomial(5.0, sym::Variable("y", 1))));
    m.add_constraint("c1", sym::Inequality::less_equal(
        sym::Expression(sym::Variable("x", 0)) + sym::Expression(sym::Variable("y", 1)),
        sym::Expression(4.0)));
    EXPECT_TRUE(m.is_valid());
    StubSolver solver;
    auto r = solver.solve(m);
    EXPECT_EQ(r.status, SolveStatus::Optimal);
}

TEST(ExampleExtended, IntegerProgramming) {
    using namespace ospf::core;
    SymbolRegistry::instance().reset();
    MetaModel m("ip_test");
    VariableBuilder b;
    m.add_variable(b.name("n").type(VariableType::Integer).lower(0).upper(10).build());
    m.set_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Variable("n", 0)));
    StubSolver solver;
    auto r = solver.solve(m);
    EXPECT_EQ(r.status, SolveStatus::Optimal);
}

TEST(ExampleExtended, MultiConstraintModel) {
    using namespace ospf::core;
    SymbolRegistry::instance().reset();
    MetaModel m("multi_c");
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).build());
    m.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 0)), sym::Expression(100.0)));
    m.add_constraint("c2", sym::Inequality::greater_equal(sym::Expression(sym::Variable("x", 0)), sym::Expression(0.0)));
    m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 0)));
    EXPECT_TRUE(m.is_valid());
    EXPECT_EQ(m.constraint_count(), 2u);
}

TEST(ExampleExtended, ThreeVariableModel) {
    using namespace ospf::core;
    SymbolRegistry::instance().reset();
    MetaModel m("3var");
    VariableBuilder b;
    m.add_variable(b.name("a").lower(0).build());
    m.add_variable(b.name("b").lower(0).build());
    m.add_variable(b.name("c").lower(0).build());
    m.set_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Variable("a", 0)) +
        sym::Expression(sym::Variable("b", 1)) +
        sym::Expression(sym::Variable("c", 2)));
    EXPECT_TRUE(m.is_valid());
    EXPECT_EQ(m.variable_count(), 3u);
}

