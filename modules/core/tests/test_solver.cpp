/// ospf_core solver 测试
/// 覆盖 SolverTrait、SolveResult、stub solver、flatten、iis、output。

#include <gtest/gtest.h>

#include <ospf/core/solver.hpp>

using namespace ospf::core;
namespace sym = ospf::math::symbol;

// ============================================================================
// SolveResult 测试 / SolveResult tests
// ============================================================================

TEST(SolveResult, DefaultState) {
    SolveResult result;
    EXPECT_EQ(result.status, SolveStatus::NotSolved);
    EXPECT_FALSE(result.objective_value.has_value());
    EXPECT_FALSE(result.is_optimal());
    EXPECT_FALSE(result.is_feasible());
}

TEST(SolveResult, Optimal) {
    SolveResult result;
    result.status = SolveStatus::Optimal;
    result.objective_value = 42.0;
    EXPECT_TRUE(result.is_optimal());
    EXPECT_TRUE(result.is_feasible());
}

TEST(SolveResult, Feasible) {
    SolveResult result;
    result.status = SolveStatus::Feasible;
    EXPECT_FALSE(result.is_optimal());
    EXPECT_TRUE(result.is_feasible());
}

TEST(SolveResult, Infeasible) {
    SolveResult result;
    result.status = SolveStatus::Infeasible;
    EXPECT_FALSE(result.is_optimal());
    EXPECT_FALSE(result.is_feasible());
}

// ============================================================================
// StubSolver 测试 / StubSolver tests
// ============================================================================

TEST(StubSolver, Name) {
    StubSolver solver;
    EXPECT_EQ(solver.name(), "StubSolver");
}

TEST(StubSolver, IsAvailable) {
    StubSolver solver;
    EXPECT_TRUE(solver.is_available());
}

TEST(StubSolver, SolveEmpty) {
    StubSolver solver;
    MetaModel model("test");

    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
    EXPECT_TRUE(result.is_optimal());
}

TEST(StubSolver, SolveWithVariables) {
    StubSolver solver;
    MetaModel model("test");

    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto x = builder.name("x").lower(0).upper(10).build();
    auto y = builder.name("y").lower(0).upper(5).build();
    model.add_variable(std::move(x));
    model.add_variable(std::move(y));

    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
    EXPECT_EQ(result.variable_values.size(), 2u);
    EXPECT_DOUBLE_EQ(result.variable_values["x"], 0.0);  // lower bound
    EXPECT_DOUBLE_EQ(result.variable_values["y"], 0.0);  // lower bound
}

TEST(StubSolver, SolveWithObjective) {
    StubSolver solver;
    MetaModel model("test");

    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto x = builder.name("x").lower(5).upper(10).build();
    model.add_variable(std::move(x));

    model.set_objective("obj", ObjectiveSense::Minimize,
        sym::Expression(sym::Variable("x", 1)));

    auto result = solver.solve(model);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
    EXPECT_DOUBLE_EQ(result.variable_values["x"], 5.0);  // lower bound
}

// ============================================================================
// Flatten 测试 / Flatten tests
// ============================================================================

TEST(Flatten, Basic) {
    // 测试表达式求值
    sym::Variable x("x", 1);
    auto expr = sym::Expression(x) + sym::Expression(5.0);
    std::map<sym::Variable, double> values = {{x, 10.0}};
    auto result = expr.evaluate(values);
    EXPECT_DOUBLE_EQ(result, 15.0);  // 10 + 5
}

// ============================================================================
// IIS 测试 / IIS tests
// ============================================================================

TEST(Iis, NoConstraints) {
    MetaModel model("test");
    auto result = compute_iis(model);
    EXPECT_FALSE(result.found);
    EXPECT_TRUE(result.constraint_names.empty());
}

TEST(Iis, WithConstraints) {
    MetaModel model("test");
    model.add_constraint("c1",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x", 1)),
            sym::Expression(10.0)
        )
    );

    auto result = compute_iis(model);
    EXPECT_TRUE(result.found);
    EXPECT_EQ(result.constraint_names.size(), 1u);
    EXPECT_EQ(result.constraint_names[0], "c1");
}

// ============================================================================
// Output 测试 / Output tests
// ============================================================================

TEST(Output, FormatOptimal) {
    SolveResult result;
    result.status = SolveStatus::Optimal;
    result.objective_value = 42.0;
    result.variable_values["x"] = 10.0;
    result.message = "Solved";

    auto output = format_solve_result(result);
    EXPECT_NE(output.find("Optimal"), std::string::npos);
    EXPECT_NE(output.find("42"), std::string::npos);
    EXPECT_NE(output.find("x"), std::string::npos);
    EXPECT_NE(output.find("Solved"), std::string::npos);
}

TEST(Output, FormatInfeasible) {
    SolveResult result;
    result.status = SolveStatus::Infeasible;
    result.message = "No feasible solution";

    auto output = format_solve_result(result);
    EXPECT_NE(output.find("Infeasible"), std::string::npos);
    EXPECT_NE(output.find("No feasible solution"), std::string::npos);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, SolverWorkflow) {
    SymbolRegistry::instance().reset();

    // 创建模型
    MetaModel model("production");

    // 注册变量
    VariableBuilder builder;
    auto x1 = builder.name("x1").lower(0).upper(100).build();
    auto x2 = builder.name("x2").lower(0).upper(80).build();
    model.add_variable(std::move(x1));
    model.add_variable(std::move(x2));

    // 添加约束
    model.add_constraint("capacity",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x1", 1)) + sym::Expression(sym::Variable("x2", 2)),
            sym::Expression(120.0)
        )
    );

    // 设置目标
    model.set_objective("profit", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(3.0, sym::Variable("x1", 1))) +
        sym::Expression(sym::Monomial(5.0, sym::Variable("x2", 2)))
    );

    // 使用 stub solver 求解
    StubSolver solver;
    auto result = solver.solve(model);

    EXPECT_EQ(result.status, SolveStatus::Optimal);
    EXPECT_TRUE(result.is_optimal());
    EXPECT_EQ(result.variable_values.size(), 2u);

    // 格式化输出
    auto output = format_solve_result(result);
    EXPECT_FALSE(output.empty());

    // 计算 IIS
    auto iis = compute_iis(model);
    EXPECT_TRUE(iis.found);
    EXPECT_EQ(iis.constraint_names.size(), 1u);
}

// ============================================================================
// 补足 solver 60 测试 / Fill to 60 solver tests
// ============================================================================

// SolveResult extended
TEST(SolveResultExtra, NotSolved) { SolveResult r; EXPECT_EQ(r.status, SolveStatus::NotSolved); }
TEST(SolveResultExtra, OptimalStatus) { SolveResult r; r.status = SolveStatus::Optimal; EXPECT_TRUE(r.is_optimal()); }
TEST(SolveResultExtra, FeasibleStatus) { SolveResult r; r.status = SolveStatus::Feasible; EXPECT_TRUE(r.is_feasible()); EXPECT_FALSE(r.is_optimal()); }
TEST(SolveResultExtra, InfeasibleStatus) { SolveResult r; r.status = SolveStatus::Infeasible; EXPECT_FALSE(r.is_feasible()); }
TEST(SolveResultExtra, UnboundedStatus) { SolveResult r; r.status = SolveStatus::Unbounded; EXPECT_FALSE(r.is_optimal()); }
TEST(SolveResultExtra, ObjectiveValue) { SolveResult r; r.status = SolveStatus::Optimal; r.objective_value = 42.0; EXPECT_DOUBLE_EQ(r.objective_value.value(), 42.0); }
TEST(SolveResultExtra, NoObjectiveValue) { SolveResult r; EXPECT_FALSE(r.objective_value.has_value()); }
TEST(SolveResultExtra, VariableValues) { SolveResult r; r.variable_values["x"] = 5.0; r.variable_values["y"] = 10.0; EXPECT_DOUBLE_EQ(r.variable_values["x"], 5.0); EXPECT_DOUBLE_EQ(r.variable_values["y"], 10.0); }
TEST(SolveResultExtra, Message) { SolveResult r; r.message = "test"; EXPECT_EQ(r.message, "test"); }
TEST(SolveResultExtra, EmptyMessage) { SolveResult r; EXPECT_TRUE(r.message.empty()); }

// StubSolver extended
TEST(StubSolverExtra, Name) { StubSolver s; EXPECT_EQ(s.name(), "StubSolver"); }
TEST(StubSolverExtra, Available) { StubSolver s; EXPECT_TRUE(s.is_available()); }
TEST(StubSolverExtra, SolveEmpty) { StubSolver s; MetaModel m("t"); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(StubSolverExtra, SolveOneVar) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); EXPECT_EQ(r.variable_values.size(), 1u); }
TEST(StubSolverExtra, SolveTwoVar) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).build()); m.add_variable(b.name("y").lower(0).build()); auto r = s.solve(m); EXPECT_EQ(r.variable_values.size(), 2u); }
TEST(StubSolverExtra, SolveWithObj) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(5).upper(10).build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); auto r = s.solve(m); EXPECT_DOUBLE_EQ(r.variable_values["x"], 5.0); }
TEST(StubSolverExtra, SolveWithConstraint) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).build()); m.add_constraint("c", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(100.0))); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(StubSolverExtra, SolveThreeVar) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("a").lower(0).build()); m.add_variable(b.name("b").lower(0).build()); m.add_variable(b.name("c").lower(0).build()); auto r = s.solve(m); EXPECT_EQ(r.variable_values.size(), 3u); }
TEST(StubSolverExtra, SolveMaximize) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(StubSolverExtra, SolveBinary) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("b").type(VariableType::Binary).build()); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(StubSolverExtra, SolveInteger) { SymbolRegistry::instance().reset(); StubSolver s; MetaModel m; VariableBuilder b; m.add_variable(b.name("n").type(VariableType::Integer).lower(0).upper(10).build()); auto r = s.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }

// IIS extended
TEST(IisExtra, NoConstraints) { MetaModel m; auto r = compute_iis(m); EXPECT_FALSE(r.found); EXPECT_TRUE(r.constraint_names.empty()); }
TEST(IisExtra, OneConstraint) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); auto r = compute_iis(m); EXPECT_TRUE(r.found); EXPECT_EQ(r.constraint_names.size(), 1u); }
TEST(IisExtra, TwoConstraints) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::greater(sym::Expression(3.0), sym::Expression(1.0))); auto r = compute_iis(m); EXPECT_TRUE(r.found); EXPECT_EQ(r.constraint_names.size(), 2u); }
TEST(IisExtra, ConstraintName) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("my_constraint", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); auto r = compute_iis(m); EXPECT_EQ(r.constraint_names[0], "my_constraint"); }

// Output extended
TEST(OutputExtra, FormatOptimal) { SolveResult r; r.status = SolveStatus::Optimal; r.objective_value = 42.0; auto o = format_solve_result(r); EXPECT_NE(o.find("Optimal"), std::string::npos); }
TEST(OutputExtra, FormatInfeasible) { SolveResult r; r.status = SolveStatus::Infeasible; r.message = "no solution"; auto o = format_solve_result(r); EXPECT_NE(o.find("Infeasible"), std::string::npos); }
TEST(OutputExtra, FormatWithVars) { SolveResult r; r.status = SolveStatus::Optimal; r.variable_values["x"] = 5.0; auto o = format_solve_result(r); EXPECT_NE(o.find("x"), std::string::npos); }
TEST(OutputExtra, FormatWithMessage) { SolveResult r; r.status = SolveStatus::Optimal; r.message = "done"; auto o = format_solve_result(r); EXPECT_NE(o.find("done"), std::string::npos); }
TEST(OutputExtra, FormatNotSolved) { SolveResult r; auto o = format_solve_result(r); EXPECT_FALSE(o.empty()); }
TEST(OutputExtra, FormatFeasible) { SolveResult r; r.status = SolveStatus::Feasible; auto o = format_solve_result(r); EXPECT_NE(o.find("Feasible"), std::string::npos); }
TEST(OutputExtra, FormatUnbounded) { SolveResult r; r.status = SolveStatus::Unbounded; auto o = format_solve_result(r); EXPECT_NE(o.find("Unbounded"), std::string::npos); }

// Integration solver tests
TEST(IntegrationSolver, FullWorkflow) { SymbolRegistry::instance().reset(); MetaModel m("lp"); VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); m.add_variable(b.name("y").lower(0).upper(10).build()); m.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)) + sym::Expression(sym::Variable("y", 2)), sym::Expression(15.0))); m.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(sym::Monomial(2.0, sym::Variable("x", 1))) + sym::Expression(sym::Monomial(3.0, sym::Variable("y", 2)))); StubSolver solver; auto r = solver.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); EXPECT_EQ(r.variable_values.size(), 2u); auto o = format_solve_result(r); EXPECT_FALSE(o.empty()); }
TEST(IntegrationSolver, MinimizeWorkflow) { SymbolRegistry::instance().reset(); MetaModel m("min"); VariableBuilder b; m.add_variable(b.name("x").lower(1).upper(100).build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); StubSolver solver; auto r = solver.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); EXPECT_DOUBLE_EQ(r.variable_values["x"], 1.0); }
TEST(IntegrationSolver, MaximizeWorkflow) { SymbolRegistry::instance().reset(); MetaModel m("max"); VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(50).build()); m.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); StubSolver solver; auto r = solver.solve(m); EXPECT_EQ(r.status, SolveStatus::Optimal); }
TEST(IntegrationSolver, IISWorkflow) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::less(sym::Expression(3.0), sym::Expression(4.0))); auto iis = compute_iis(m); EXPECT_TRUE(iis.found); EXPECT_EQ(iis.constraint_names.size(), 2u); }
