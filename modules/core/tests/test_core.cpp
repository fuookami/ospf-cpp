/// ospf_core 测试
/// 覆盖符号、变量、模型。

#include <gtest/gtest.h>

#include <ospf/core/symbol.hpp>
#include <ospf/core/variable.hpp>
#include <ospf/core/model.hpp>

using namespace ospf::core;
namespace sym = ospf::math::symbol;

// ============================================================================
// Symbol 测试 / Symbol tests
// ============================================================================

TEST(Symbol, Creation) {
    SymbolRegistry::instance().reset();
    auto sym = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable);
    EXPECT_EQ(sym.name(), "x");
    EXPECT_EQ(sym.type(), SymbolType::Variable);
    EXPECT_EQ(sym.id(), 0u);
}

TEST(Symbol, UniqueIds) {
    SymbolRegistry::instance().reset();
    auto s1 = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable);
    auto s2 = SymbolRegistry::instance().create_symbol("y", SymbolType::Variable);
    EXPECT_NE(s1.id(), s2.id());
}

TEST(Symbol, Equality) {
    SymbolRegistry::instance().reset();
    auto s1 = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable);
    auto s2 = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable);
    EXPECT_NE(s1, s2);  // 不同创建 = 不同符号
}

TEST(Symbol, Types) {
    SymbolRegistry::instance().reset();
    auto var = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable);
    auto con = SymbolRegistry::instance().create_symbol("c", SymbolType::Constraint);
    auto obj = SymbolRegistry::instance().create_symbol("obj", SymbolType::Objective);

    EXPECT_EQ(var.type(), SymbolType::Variable);
    EXPECT_EQ(con.type(), SymbolType::Constraint);
    EXPECT_EQ(obj.type(), SymbolType::Objective);
}

// ============================================================================
// Variable 测试 / Variable tests
// ============================================================================

TEST(Variable, Construction) {
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto var = builder.name("x").lower(0.0).upper(10.0).build();

    EXPECT_EQ(var.name(), "x");
    EXPECT_EQ(var.type(), VariableType::Continuous);
    EXPECT_DOUBLE_EQ(var.bounds().lower, 0.0);
    EXPECT_DOUBLE_EQ(var.bounds().upper, 10.0);
}

TEST(Variable, IntegerType) {
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto var = builder.name("n").type(VariableType::Integer).lower(0).upper(100).build();

    EXPECT_EQ(var.type(), VariableType::Integer);
}

TEST(Variable, BinaryType) {
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto var = builder.name("b").type(VariableType::Binary).build();

    EXPECT_EQ(var.type(), VariableType::Binary);
}

TEST(Variable, InitialValue) {
    SymbolRegistry::instance().reset();
    VariableBuilder builder;
    auto var = builder.name("x").initial(5.0).build();

    EXPECT_TRUE(var.initial().has_value());
    EXPECT_DOUBLE_EQ(var.initial().value(), 5.0);
}

TEST(Variable, Bounds) {
    VariableBounds b1{0.0, 10.0};
    EXPECT_TRUE(b1.is_valid());

    VariableBounds b2{10.0, 0.0};
    EXPECT_FALSE(b2.is_valid());
}

// ============================================================================
// MetaModel 测试 / MetaModel tests
// ============================================================================

TEST(MetaModel, Creation) {
    MetaModel model("test");
    EXPECT_EQ(model.name(), "test");
    EXPECT_EQ(model.stage(), ModelStage::Registration);
    EXPECT_EQ(model.variable_count(), 0u);
    EXPECT_FALSE(model.is_valid());
}

TEST(MetaModel, AddVariable) {
    MetaModel model;
    SymbolRegistry::instance().reset();

    VariableBuilder builder;
    auto var = builder.name("x").lower(0).upper(10).build();
    model.add_variable(std::move(var));

    EXPECT_EQ(model.variable_count(), 1u);
    EXPECT_EQ(model.variable(0).name(), "x");
}

TEST(MetaModel, AddConstraint) {
    MetaModel model;
    SymbolRegistry::instance().reset();

    VariableBuilder builder;
    auto x = builder.name("x").build();

    // x <= 10
    model.add_constraint("c1",
        sym::Inequality::less_equal(
            sym::Expression(sym::Monomial(1.0, sym::Variable("x", 1))),
            sym::Expression(10.0)
        )
    );

    EXPECT_EQ(model.constraint_count(), 1u);
}

TEST(MetaModel, SetObjective) {
    MetaModel model;
    SymbolRegistry::instance().reset();

    // 添加变量（is_valid 需要至少一个变量）
    VariableBuilder builder;
    auto x = builder.name("x").build();
    model.add_variable(std::move(x));

    // minimize x
    model.set_objective("obj", ObjectiveSense::Minimize,
        sym::Expression(sym::Variable("x", 1)));

    EXPECT_TRUE(model.objective().has_value());
    EXPECT_EQ(model.objective()->sense, ObjectiveSense::Minimize);
    EXPECT_TRUE(model.is_valid());
}

TEST(MetaModel, StageManagement) {
    MetaModel model;
    EXPECT_EQ(model.stage(), ModelStage::Registration);

    model.set_stage(ModelStage::Building);
    EXPECT_EQ(model.stage(), ModelStage::Building);

    model.set_stage(ModelStage::Solving);
    EXPECT_EQ(model.stage(), ModelStage::Solving);

    model.set_stage(ModelStage::Solved);
    EXPECT_EQ(model.stage(), ModelStage::Solved);
}

TEST(MetaModel, IsValid) {
    MetaModel model;
    SymbolRegistry::instance().reset();

    // 没有变量和目标 → 无效
    EXPECT_FALSE(model.is_valid());

    // 添加变量
    VariableBuilder builder;
    auto x = builder.name("x").build();
    model.add_variable(std::move(x));
    EXPECT_FALSE(model.is_valid());  // 仍然没有目标

    // 添加目标
    model.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0));
    EXPECT_TRUE(model.is_valid());
}

// ============================================================================
// AbstractLinearMetaModel 测试 / Abstract linear model tests
// ============================================================================

TEST(AbstractLinearMetaModel, Creation) {
    AbstractLinearMetaModel model("linear_test");
    EXPECT_EQ(model.name(), "linear_test");
}

TEST(AbstractLinearMetaModel, AddLinearConstraint) {
    AbstractLinearMetaModel model;
    SymbolRegistry::instance().reset();

    model.add_linear_constraint("c1",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x", 1)),
            sym::Expression(10.0)
        )
    );

    EXPECT_EQ(model.constraint_count(), 1u);
}

TEST(AbstractLinearMetaModel, SetLinearObjective) {
    AbstractLinearMetaModel model;
    SymbolRegistry::instance().reset();

    model.set_linear_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Variable("x", 1)));

    EXPECT_TRUE(model.objective().has_value());
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, ModelWorkflow) {
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
    // x1 + x2 <= 120
    model.add_constraint("capacity",
        sym::Inequality::less_equal(
            sym::Expression(sym::Variable("x1", 1)) + sym::Expression(sym::Variable("x2", 2)),
            sym::Expression(120.0)
        )
    );

    // 设置目标：最大化 3*x1 + 5*x2
    model.set_objective("profit", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(3.0, sym::Variable("x1", 1))) +
        sym::Expression(sym::Monomial(5.0, sym::Variable("x2", 2)))
    );

    // 验证
    EXPECT_EQ(model.variable_count(), 2u);
    EXPECT_EQ(model.constraint_count(), 1u);
    EXPECT_TRUE(model.is_valid());
    EXPECT_EQ(model.stage(), ModelStage::Registration);

    // 切换到构建阶段
    model.set_stage(ModelStage::Building);
    EXPECT_EQ(model.stage(), ModelStage::Building);
}
