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
