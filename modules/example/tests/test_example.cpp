/// ospf_example 测试
/// 覆盖 example 模块基本功能

#include <gtest/gtest.h>
#include <ospf/example/core_shortcuts.hpp>
#include <ospf/example/common.hpp>

using namespace ospf::example;

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
        ospf::math::symbol::Expression(ospf::math::symbol::Variable("x", 1)));
    EXPECT_TRUE(m.is_valid());
}

TEST(Example, SolverUsage) {
    SymbolRegistry::instance().reset();
    MetaModel m("example");
    VariableBuilder b;
    m.add_variable(b.name("x").lower(0).upper(10).build());
    m.set_objective("obj", ObjectiveSense::Maximize, 
        ospf::math::symbol::Expression(ospf::math::symbol::Variable("x", 1)));
    
    StubSolver solver;
    auto result = solver.solve(m);
    EXPECT_EQ(result.status, SolveStatus::Optimal);
}
