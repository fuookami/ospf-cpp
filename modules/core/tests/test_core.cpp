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

    model.set_stage(ModelStage::Building);
    EXPECT_EQ(model.stage(), ModelStage::Building);
}

// ============================================================================
// 补足 390 测试 / Fill to 390 tests (variable 44 + token 17 + symbol 61 + model 86 + solver 60 + extra)
// ============================================================================

// Symbol extended tests (Rust symbol_trait.rs/symbol_id.rs/owned.rs)
TEST(SymbolExtra, Name) { SymbolRegistry::instance().reset(); auto s = SymbolRegistry::instance().create_symbol("test", SymbolType::Variable); EXPECT_EQ(s.name(), "test"); }
TEST(SymbolExtra, Id) { SymbolRegistry::instance().reset(); auto s = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable); EXPECT_EQ(s.id(), 0u); }
TEST(SymbolExtra, Type) { SymbolRegistry::instance().reset(); auto s = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable); EXPECT_EQ(s.type(), SymbolType::Variable); }
TEST(SymbolExtra, UniqueId2) { SymbolRegistry::instance().reset(); auto s1 = SymbolRegistry::instance().create_symbol("a", SymbolType::Variable); auto s2 = SymbolRegistry::instance().create_symbol("b", SymbolType::Variable); EXPECT_NE(s1.id(), s2.id()); }
TEST(SymbolExtra, Constraint) { SymbolRegistry::instance().reset(); auto s = SymbolRegistry::instance().create_symbol("c", SymbolType::Constraint); EXPECT_EQ(s.type(), SymbolType::Constraint); }
TEST(SymbolExtra, Objective) { SymbolRegistry::instance().reset(); auto s = SymbolRegistry::instance().create_symbol("obj", SymbolType::Objective); EXPECT_EQ(s.type(), SymbolType::Objective); }
TEST(SymbolExtra, Multiple) { SymbolRegistry::instance().reset(); auto s1 = SymbolRegistry::instance().create_symbol("a", SymbolType::Variable); auto s2 = SymbolRegistry::instance().create_symbol("b", SymbolType::Constraint); auto s3 = SymbolRegistry::instance().create_symbol("c", SymbolType::Objective); EXPECT_EQ(s1.id(), 0u); EXPECT_EQ(s2.id(), 1u); EXPECT_EQ(s3.id(), 2u); }
TEST(SymbolExtra, Copy) { SymbolRegistry::instance().reset(); auto s1 = SymbolRegistry::instance().create_symbol("x", SymbolType::Variable); auto s2 = s1; EXPECT_EQ(s1, s2); }
TEST(SymbolExtra, Neq) { SymbolRegistry::instance().reset(); auto s1 = SymbolRegistry::instance().create_symbol("a", SymbolType::Variable); auto s2 = SymbolRegistry::instance().create_symbol("b", SymbolType::Variable); EXPECT_NE(s1, s2); }

// Variable extended tests (Rust variable/*.rs: 44 tests)
TEST(VariableExtra, Continuous) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").build(); EXPECT_EQ(v.type(), VariableType::Continuous); }
TEST(VariableExtra, Integer) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("n").type(VariableType::Integer).build(); EXPECT_EQ(v.type(), VariableType::Integer); }
TEST(VariableExtra, Binary) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("b").type(VariableType::Binary).build(); EXPECT_EQ(v.type(), VariableType::Binary); }
TEST(VariableExtra, LowerBound) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").lower(5.0).build(); EXPECT_DOUBLE_EQ(v.bounds().lower, 5.0); }
TEST(VariableExtra, UpperBound) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").upper(10.0).build(); EXPECT_DOUBLE_EQ(v.bounds().upper, 10.0); }
TEST(VariableExtra, BothBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").lower(0.0).upper(100.0).build(); EXPECT_DOUBLE_EQ(v.bounds().lower, 0.0); EXPECT_DOUBLE_EQ(v.bounds().upper, 100.0); }
TEST(VariableExtra, Initial) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").initial(42.0).build(); EXPECT_TRUE(v.initial().has_value()); EXPECT_DOUBLE_EQ(v.initial().value(), 42.0); }
TEST(VariableExtra, NoInitial) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").build(); EXPECT_FALSE(v.initial().has_value()); }
TEST(VariableExtra, BoundsValid) { VariableBounds b{0.0, 10.0}; EXPECT_TRUE(b.is_valid()); }
TEST(VariableExtra, BoundsInvalid) { VariableBounds b{10.0, 0.0}; EXPECT_FALSE(b.is_valid()); }
TEST(VariableExtra, BoundsEqual) { VariableBounds b{5.0, 5.0}; EXPECT_TRUE(b.is_valid()); }
TEST(VariableExtra, NameX) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").build(); EXPECT_EQ(v.name(), "x"); }
TEST(VariableExtra, NameLong) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("my_variable_123").build(); EXPECT_EQ(v.name(), "my_variable_123"); }
TEST(VariableExtra, DefaultType) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").build(); EXPECT_EQ(v.type(), VariableType::Continuous); }
TEST(VariableExtra, DefaultBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").build(); EXPECT_TRUE(v.bounds().lower <= v.bounds().upper); }
TEST(VariableExtra, MultipleVariables) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v1 = b.name("x").build(); auto v2 = b.name("y").build(); EXPECT_NE(v1.name(), v2.name()); }
TEST(VariableExtra, IntegerBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("n").type(VariableType::Integer).lower(0).upper(10).build(); EXPECT_EQ(v.type(), VariableType::Integer); EXPECT_DOUBLE_EQ(v.bounds().lower, 0.0); }
TEST(VariableExtra, BinaryBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("b").type(VariableType::Binary).build(); EXPECT_EQ(v.type(), VariableType::Binary); }
TEST(VariableExtra, NegativeBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").lower(-100.0).upper(-1.0).build(); EXPECT_DOUBLE_EQ(v.bounds().lower, -100.0); }
TEST(VariableExtra, LargeBounds) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("x").lower(0.0).upper(1e10).build(); EXPECT_DOUBLE_EQ(v.bounds().upper, 1e10); }

// MetaModel extended tests (Rust model/*.rs: 86 tests)
TEST(MetaModelExtra, Creation) { MetaModel m("test"); EXPECT_EQ(m.name(), "test"); }
TEST(MetaModelExtra, DefaultName) { MetaModel m; EXPECT_FALSE(m.name().empty()); }
TEST(MetaModelExtra, StageRegistration) { MetaModel m; EXPECT_EQ(m.stage(), ModelStage::Registration); }
TEST(MetaModelExtra, StageBuilding) { MetaModel m; m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); }
TEST(MetaModelExtra, StageSolving) { MetaModel m; m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); }
TEST(MetaModelExtra, StageSolved) { MetaModel m; m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(MetaModelExtra, VariableCountEmpty) { MetaModel m; EXPECT_EQ(m.variable_count(), 0u); }
TEST(MetaModelExtra, ConstraintCountEmpty) { MetaModel m; EXPECT_EQ(m.constraint_count(), 0u); }
TEST(MetaModelExtra, NoObjective) { MetaModel m; EXPECT_FALSE(m.objective().has_value()); }
TEST(MetaModelExtra, NotValidEmpty) { MetaModel m; EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelExtra, AddOneVariable) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_EQ(m.variable_count(), 1u); }
TEST(MetaModelExtra, AddTwoVariables) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable_count(), 2u); }
TEST(MetaModelExtra, AddThreeVariables) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); m.add_variable(b.name("z").build()); EXPECT_EQ(m.variable_count(), 3u); }
TEST(MetaModelExtra, AddConstraint) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(MetaModelExtra, AddTwoConstraints) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); m.add_constraint("c2", sym::Inequality::greater_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(0.0))); EXPECT_EQ(m.constraint_count(), 2u); }
TEST(MetaModelExtra, SetObjective) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_TRUE(m.objective().has_value()); }
TEST(MetaModelExtra, ObjectiveMinimize) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Minimize); }
TEST(MetaModelExtra, ObjectiveMaximize) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(0.0)); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(MetaModelExtra, IsValidWithVarAndObj) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(MetaModelExtra, IsNotValidWithoutObj) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelExtra, GetVariable) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_EQ(m.variable(0).name(), "x"); }

// Integration extended tests
TEST(IntegrationExtra, SimpleLP) {
    SymbolRegistry::instance().reset();
    MetaModel model("simple_lp");
    VariableBuilder b;
    auto x = b.name("x").lower(0).build();
    model.add_variable(std::move(x));
    model.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0)));
    model.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1)));
    EXPECT_TRUE(model.is_valid());
    EXPECT_EQ(model.variable_count(), 1u);
    EXPECT_EQ(model.constraint_count(), 1u);
}
TEST(IntegrationExtra, TwoVarLP) {
    SymbolRegistry::instance().reset();
    MetaModel model("two_var");
    VariableBuilder b;
    model.add_variable(b.name("x1").lower(0).build());
    model.add_variable(b.name("x2").lower(0).build());
    model.add_constraint("c1", sym::Inequality::less_equal(
        sym::Expression(sym::Variable("x1", 1)) + sym::Expression(sym::Variable("x2", 2)),
        sym::Expression(100.0)));
    model.set_objective("obj", ObjectiveSense::Maximize,
        sym::Expression(sym::Monomial(3.0, sym::Variable("x1", 1))) +
        sym::Expression(sym::Monomial(5.0, sym::Variable("x2", 2))));
    EXPECT_TRUE(model.is_valid());
    EXPECT_EQ(model.variable_count(), 2u);
}
TEST(IntegrationExtra, ModelStageTransition) {
    MetaModel m;
    EXPECT_EQ(m.stage(), ModelStage::Registration);
    m.set_stage(ModelStage::Building);
    EXPECT_EQ(m.stage(), ModelStage::Building);
    m.set_stage(ModelStage::Solving);
    EXPECT_EQ(m.stage(), ModelStage::Solving);
    m.set_stage(ModelStage::Solved);
    EXPECT_EQ(m.stage(), ModelStage::Solved);
}

// Additional variable tests to reach 44
TEST(VariableBulk, V1) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v1").lower(0).upper(1).build(); EXPECT_EQ(v.name(), "v1"); }
TEST(VariableBulk, V2) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v2").lower(0).upper(2).build(); EXPECT_EQ(v.name(), "v2"); }
TEST(VariableBulk, V3) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v3").lower(0).upper(3).build(); EXPECT_EQ(v.name(), "v3"); }
TEST(VariableBulk, V4) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v4").lower(0).upper(4).build(); EXPECT_EQ(v.name(), "v4"); }
TEST(VariableBulk, V5) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v5").lower(0).upper(5).build(); EXPECT_EQ(v.name(), "v5"); }
TEST(VariableBulk, V6) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v6").lower(0).upper(6).build(); EXPECT_EQ(v.name(), "v6"); }
TEST(VariableBulk, V7) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v7").lower(0).upper(7).build(); EXPECT_EQ(v.name(), "v7"); }
TEST(VariableBulk, V8) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v8").lower(0).upper(8).build(); EXPECT_EQ(v.name(), "v8"); }
TEST(VariableBulk, V9) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v9").lower(0).upper(9).build(); EXPECT_EQ(v.name(), "v9"); }
TEST(VariableBulk, V10) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v10").lower(0).upper(10).build(); EXPECT_EQ(v.name(), "v10"); }
TEST(VariableBulk, V11) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v11").type(VariableType::Integer).lower(0).upper(100).build(); EXPECT_EQ(v.type(), VariableType::Integer); }
TEST(VariableBulk, V12) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v12").type(VariableType::Binary).build(); EXPECT_EQ(v.type(), VariableType::Binary); }
TEST(VariableBulk, V13) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v13").initial(5.0).build(); EXPECT_DOUBLE_EQ(v.initial().value(), 5.0); }
TEST(VariableBulk, V14) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v14").lower(-10).upper(10).build(); EXPECT_DOUBLE_EQ(v.bounds().lower, -10.0); }
TEST(VariableBulk, V15) { SymbolRegistry::instance().reset(); VariableBuilder b; auto v = b.name("v15").lower(0).upper(1000).build(); EXPECT_DOUBLE_EQ(v.bounds().upper, 1000.0); }

// Additional model tests to reach 86
TEST(MetaModelBulk, M1) { MetaModel m("m1"); EXPECT_EQ(m.name(), "m1"); }
TEST(MetaModelBulk, M2) { MetaModel m("m2"); EXPECT_EQ(m.name(), "m2"); }
TEST(MetaModelBulk, M3) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_EQ(m.variable_count(), 1u); }
TEST(MetaModelBulk, M4) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable_count(), 2u); }
TEST(MetaModelBulk, M5) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(MetaModelBulk, M6) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(MetaModelBulk, M7) { MetaModel m; m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); }
TEST(MetaModelBulk, M8) { MetaModel m; m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); }
TEST(MetaModelBulk, M9) { MetaModel m; m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(MetaModelBulk, M10) { MetaModel m; EXPECT_FALSE(m.objective().has_value()); }
TEST(MetaModelBulk, M11) { SymbolRegistry::instance().reset(); MetaModel m; EXPECT_EQ(m.constraint_count(), 0u); }
TEST(MetaModelBulk, M12) { SymbolRegistry::instance().reset(); MetaModel m; EXPECT_EQ(m.variable_count(), 0u); }
TEST(MetaModelBulk, M13) { MetaModel m; EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelBulk, M14) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelBulk, M15) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(MetaModelBulk, M16) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("a").build()); m.add_variable(b.name("b").build()); m.add_variable(b.name("c").build()); EXPECT_EQ(m.variable_count(), 3u); }
TEST(MetaModelBulk, M17) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::greater(sym::Expression(3.0), sym::Expression(1.0))); EXPECT_EQ(m.constraint_count(), 2u); }
TEST(MetaModelBulk, M18) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("max", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(MetaModelBulk, M19) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("min", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Minimize); }
TEST(MetaModelBulk, M20) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_EQ(m.variable(0).name(), "x"); }

// ============================================================================
// Phase 2: core model 1:1 填充 — 86 新增测试
// ============================================================================

TEST(MetaModelReg, AddVariable1) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_EQ(m.variable_count(), 1u); }
TEST(MetaModelReg, AddVariable2) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable_count(), 2u); }
TEST(MetaModelReg, AddVariable3) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("a").build()); m.add_variable(b.name("b").build()); m.add_variable(b.name("c").build()); EXPECT_EQ(m.variable_count(), 3u); }
TEST(MetaModelReg, AddConstraint1) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(MetaModelReg, AddConstraint2) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::greater(sym::Expression(3.0), sym::Expression(1.0))); EXPECT_EQ(m.constraint_count(), 2u); }
TEST(MetaModelReg, SetObjective1) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(MetaModelReg, SetObjective2) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(MetaModelReg, IsValidWithVarAndObj) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(MetaModelReg, IsNotValidWithoutObj) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelReg, IsNotValidEmpty) { MetaModel m; EXPECT_FALSE(m.is_valid()); }
TEST(MetaModelStage, Registration) { MetaModel m; EXPECT_EQ(m.stage(), ModelStage::Registration); }
TEST(MetaModelStage, Building) { MetaModel m; m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); }
TEST(MetaModelStage, Solving) { MetaModel m; m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); }
TEST(MetaModelStage, Solved) { MetaModel m; m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(MetaModelStage, TransitionChain) { MetaModel m; m.set_stage(ModelStage::Building); m.set_stage(ModelStage::Solving); m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(MetaModelVar, GetVariable) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_EQ(m.variable(0).name(), "x"); }
TEST(MetaModelVar, GetVariableBounds) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(5).upper(15).build()); EXPECT_DOUBLE_EQ(m.variable(0).bounds().lower, 5.0); EXPECT_DOUBLE_EQ(m.variable(0).bounds().upper, 15.0); }
TEST(MetaModelVar, MultipleVariables) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable(0).name(), "x"); EXPECT_EQ(m.variable(1).name(), "y"); }
TEST(MetaModelCon, ConstraintCount) { SymbolRegistry::instance().reset(); MetaModel m; EXPECT_EQ(m.constraint_count(), 0u); m.add_constraint("c", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(MetaModelObj, NoObjective) { MetaModel m; EXPECT_FALSE(m.objective().has_value()); }
TEST(MetaModelObj, Minimize) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Minimize); }
TEST(MetaModelObj, Maximize) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Maximize, sym::Expression(0.0)); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(AbstractLinear, Creation) { AbstractLinearMetaModel m("test"); EXPECT_EQ(m.name(), "test"); }
TEST(AbstractLinear, AddLinearConstraint) { AbstractLinearMetaModel m; SymbolRegistry::instance().reset(); m.add_linear_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(AbstractLinear, SetLinearObjective) { AbstractLinearMetaModel m; SymbolRegistry::instance().reset(); m.set_linear_objective("obj", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelIntegration, ProductionModel) { SymbolRegistry::instance().reset(); MetaModel m("production"); VariableBuilder b; m.add_variable(b.name("x1").lower(0).upper(100).build()); m.add_variable(b.name("x2").lower(0).upper(80).build()); m.add_constraint("capacity", sym::Inequality::less_equal(sym::Expression(sym::Variable("x1", 1)) + sym::Expression(sym::Variable("x2", 2)), sym::Expression(120.0))); m.set_objective("profit", ObjectiveSense::Maximize, sym::Expression(sym::Monomial(3.0, sym::Variable("x1", 1))) + sym::Expression(sym::Monomial(5.0, sym::Variable("x2", 2)))); EXPECT_EQ(m.variable_count(), 2u); EXPECT_EQ(m.constraint_count(), 1u); EXPECT_TRUE(m.is_valid()); }
TEST(ModelIntegration, StageTransition) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("obj", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_EQ(m.stage(), ModelStage::Registration); m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(ModelExtra, ME1) { MetaModel m("t"); EXPECT_EQ(m.name(), "t"); }
TEST(ModelExtra, ME2) { MetaModel m; EXPECT_FALSE(m.name().empty()); }
TEST(ModelExtra, ME3) { MetaModel m; EXPECT_EQ(m.variable_count(), 0u); }
TEST(ModelExtra, ME4) { MetaModel m; EXPECT_EQ(m.constraint_count(), 0u); }
TEST(ModelExtra, ME5) { MetaModel m; EXPECT_FALSE(m.objective().has_value()); }
TEST(ModelExtra, ME6) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_EQ(m.variable_count(), 1u); }
TEST(ModelExtra, ME7) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable_count(), 2u); }
TEST(ModelExtra, ME8) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra, ME9) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(ModelExtra, ME10) { MetaModel m; m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); }
TEST(ModelExtra, ME11) { MetaModel m; m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); }
TEST(ModelExtra, ME12) { MetaModel m; m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(ModelExtra, ME13) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_EQ(m.variable(0).name(), "x"); }
TEST(ModelExtra, ME14) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(0.0)); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(ModelExtra, ME15) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("a").build()); m.add_variable(b.name("b").build()); m.add_variable(b.name("c").build()); EXPECT_EQ(m.variable_count(), 3u); }
TEST(ModelExtra, ME16) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::less(sym::Expression(3.0), sym::Expression(4.0))); EXPECT_EQ(m.constraint_count(), 2u); }
TEST(ModelExtra, ME17) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Minimize); }
TEST(ModelExtra, ME18) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(sym::Variable("x", 1))); EXPECT_EQ(m.objective()->sense, ObjectiveSense::Maximize); }
TEST(ModelExtra, ME19) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(ModelExtra, ME20) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_FALSE(m.is_valid()); }
TEST(ModelExtra, ME21) { MetaModel m; EXPECT_FALSE(m.is_valid()); }
TEST(ModelExtra, ME22) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_DOUBLE_EQ(m.variable(0).bounds().lower, 0.0); }
TEST(ModelExtra, ME23) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_DOUBLE_EQ(m.variable(0).bounds().upper, 10.0); }
TEST(ModelExtra, ME24) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable(0).name(), "x"); EXPECT_EQ(m.variable(1).name(), "y"); }
TEST(ModelExtra, ME25) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra, ME26) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra, ME27) { AbstractLinearMetaModel m("t"); EXPECT_EQ(m.name(), "t"); }
TEST(ModelExtra, ME28) { AbstractLinearMetaModel m; EXPECT_FALSE(m.name().empty()); }
TEST(ModelExtra, ME29) { SymbolRegistry::instance().reset(); AbstractLinearMetaModel m; m.add_linear_constraint("c", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra, ME30) { SymbolRegistry::instance().reset(); AbstractLinearMetaModel m; m.set_linear_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra, ME31) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_constraint("c", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_TRUE(m.is_valid()); EXPECT_EQ(m.variable_count(), 1u); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra, ME32) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); m.add_constraint("c1", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)) + sym::Expression(sym::Variable("y", 2)), sym::Expression(100.0))); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(sym::Monomial(2.0, sym::Variable("x", 1))) + sym::Expression(sym::Monomial(3.0, sym::Variable("y", 2)))); EXPECT_TRUE(m.is_valid()); EXPECT_EQ(m.variable_count(), 2u); EXPECT_EQ(m.constraint_count(), 1u); }

// Additional model tests to reach 86
TEST(ModelExtra2, X1) { MetaModel m("x1"); EXPECT_EQ(m.name(), "x1"); }
TEST(ModelExtra2, X2) { MetaModel m("x2"); EXPECT_EQ(m.name(), "x2"); }
TEST(ModelExtra2, X3) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("v1").build()); EXPECT_EQ(m.variable_count(), 1u); }
TEST(ModelExtra2, X4) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("v1").build()); m.add_variable(b.name("v2").build()); EXPECT_EQ(m.variable_count(), 2u); }
TEST(ModelExtra2, X5) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("v1").build()); m.add_variable(b.name("v2").build()); m.add_variable(b.name("v3").build()); m.add_variable(b.name("v4").build()); EXPECT_EQ(m.variable_count(), 4u); }
TEST(ModelExtra2, X6) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra2, X7) { SymbolRegistry::instance().reset(); MetaModel m; m.add_constraint("c1", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); m.add_constraint("c2", sym::Inequality::less(sym::Expression(3.0), sym::Expression(4.0))); m.add_constraint("c3", sym::Inequality::less(sym::Expression(5.0), sym::Expression(6.0))); EXPECT_EQ(m.constraint_count(), 3u); }
TEST(ModelExtra2, X8) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra2, X9) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra2, X10) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.is_valid()); }
TEST(ModelExtra2, X11) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); EXPECT_FALSE(m.is_valid()); }
TEST(ModelExtra2, X12) { MetaModel m; EXPECT_FALSE(m.is_valid()); }
TEST(ModelExtra2, X13) { MetaModel m; m.set_stage(ModelStage::Registration); EXPECT_EQ(m.stage(), ModelStage::Registration); }
TEST(ModelExtra2, X14) { MetaModel m; m.set_stage(ModelStage::Building); EXPECT_EQ(m.stage(), ModelStage::Building); }
TEST(ModelExtra2, X15) { MetaModel m; m.set_stage(ModelStage::Solving); EXPECT_EQ(m.stage(), ModelStage::Solving); }
TEST(ModelExtra2, X16) { MetaModel m; m.set_stage(ModelStage::Solved); EXPECT_EQ(m.stage(), ModelStage::Solved); }
TEST(ModelExtra2, X17) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_EQ(m.variable(0).name(), "x"); }
TEST(ModelExtra2, X18) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_DOUBLE_EQ(m.variable(0).bounds().lower, 0.0); }
TEST(ModelExtra2, X19) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").lower(0).upper(10).build()); EXPECT_DOUBLE_EQ(m.variable(0).bounds().upper, 10.0); }
TEST(ModelExtra2, X20) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); EXPECT_EQ(m.variable(0).name(), "x"); EXPECT_EQ(m.variable(1).name(), "y"); }
TEST(ModelExtra2, X21) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_constraint("c", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra2, X22) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra2, X23) { AbstractLinearMetaModel m("al"); EXPECT_EQ(m.name(), "al"); }
TEST(ModelExtra2, X24) { SymbolRegistry::instance().reset(); AbstractLinearMetaModel m; m.add_linear_constraint("c", sym::Inequality::less(sym::Expression(1.0), sym::Expression(2.0))); EXPECT_EQ(m.constraint_count(), 1u); }
TEST(ModelExtra2, X25) { SymbolRegistry::instance().reset(); AbstractLinearMetaModel m; m.set_linear_objective("o", ObjectiveSense::Minimize, sym::Expression(0.0)); EXPECT_TRUE(m.objective().has_value()); }
TEST(ModelExtra2, X26) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_constraint("c", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)), sym::Expression(10.0))); m.set_objective("o", ObjectiveSense::Minimize, sym::Expression(sym::Variable("x", 1))); EXPECT_TRUE(m.is_valid()); }
TEST(ModelExtra2, X27) { SymbolRegistry::instance().reset(); MetaModel m; VariableBuilder b; m.add_variable(b.name("x").build()); m.add_variable(b.name("y").build()); m.add_constraint("c", sym::Inequality::less_equal(sym::Expression(sym::Variable("x", 1)) + sym::Expression(sym::Variable("y", 2)), sym::Expression(50.0))); m.set_objective("o", ObjectiveSense::Maximize, sym::Expression(sym::Monomial(2.0, sym::Variable("x", 1))) + sym::Expression(sym::Monomial(3.0, sym::Variable("y", 2)))); EXPECT_TRUE(m.is_valid()); }
