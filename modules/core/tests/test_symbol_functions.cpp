/// ospf_core symbol/functions 测试
/// 1:1 移植 Rust core/symbol/functions 全部 #[test]（165 测试）
/// 覆盖：abs/and/big_m/binaryzation/ceiling/cos/first/floor/if_function/if_in/if_then/imply/in_step_range/inequality/masking/max/min_max/mod_function/one_of/product/quadratic_*/rounding/same_as/satisfied_amount/semantic/semi/sigmoid/sin/slack/slack_range/univariate_linear_piecewise

#include <gtest/gtest.h>

#include <ospf/core/symbol/functions/mod.hpp>

#include <numbers>

using namespace ospf::core;

// ============================================================================
// Abs function tests (Rust abs.rs)
// ============================================================================

TEST(SymbolFunctionAbs, Name) {
    AbsFunction<> func;
    EXPECT_STREQ(func.name(), "abs");
}

TEST(SymbolFunctionAbs, EvaluatePositive) {
    AbsFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionAbs, EvaluateNegative) {
    AbsFunction<> func;
    auto result = func.evaluate_single(-3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionAbs, EvaluateZero) {
    AbsFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// And function tests (Rust and.rs)
// ============================================================================

TEST(SymbolFunctionAnd, Name) {
    AndFunction<> func;
    EXPECT_STREQ(func.name(), "and");
}

TEST(SymbolFunctionAnd, TrueAndTrue) {
    AndFunction<> func;
    auto result = func.evaluate_binary(1.0, 1.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionAnd, TrueAndFalse) {
    AndFunction<> func;
    auto result = func.evaluate_binary(1.0, 0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionAnd, FalseAndFalse) {
    AndFunction<> func;
    auto result = func.evaluate_binary(0.0, 0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// BalanceTernaryzation tests (Rust balance_ternaryzation.rs)
// ============================================================================

TEST(SymbolFunctionBalanceTernaryzation, Name) {
    BalanceTernaryzationFunction<> func;
    EXPECT_STREQ(func.name(), "balance_ternaryzation");
}

TEST(SymbolFunctionBalanceTernaryzation, Positive) {
    BalanceTernaryzationFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionBalanceTernaryzation, Negative) {
    BalanceTernaryzationFunction<> func;
    auto result = func.evaluate_single(-3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, -1.0);
}

TEST(SymbolFunctionBalanceTernaryzation, Zero) {
    BalanceTernaryzationFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// BigM tests (Rust big_m.rs)
// ============================================================================

TEST(SymbolFunctionBigM, Name) {
    BigMFunction<> func;
    EXPECT_STREQ(func.name(), "big_m");
}

TEST(SymbolFunctionBigM, Evaluate) {
    BigMFunction<> func;
    auto result = func.evaluate({42.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 42.0);
}

TEST(SymbolFunctionBigM, Empty) {
    BigMFunction<> func;
    auto result = func.evaluate({});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Binaryzation tests (Rust binaryzation.rs)
// ============================================================================

TEST(SymbolFunctionBinaryzation, Name) {
    BinaryzationFunction<> func;
    EXPECT_STREQ(func.name(), "binaryzation");
}

TEST(SymbolFunctionBinaryzation, NonZero) {
    BinaryzationFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionBinaryzation, Zero) {
    BinaryzationFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// BivariateLinearPiecewise tests (Rust bivariate_linear_piecewise.rs)
// ============================================================================

TEST(SymbolFunctionBivariateLinearPiecewise, Name) {
    BivariateLinearPiecewiseFunction<> func;
    EXPECT_STREQ(func.name(), "bivariate_linear_piecewise");
}

TEST(SymbolFunctionBivariateLinearPiecewise, Evaluate) {
    BivariateLinearPiecewiseFunction<> func;
    auto result = func.evaluate({3.0, 4.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 7.0);
}

// ============================================================================
// Ceiling tests (Rust ceiling.rs)
// ============================================================================

TEST(SymbolFunctionCeiling, Name) {
    CeilingFunction<> func;
    EXPECT_STREQ(func.name(), "ceiling");
}

TEST(SymbolFunctionCeiling, Evaluate) {
    CeilingFunction<> func;
    auto result = func.evaluate_single(3.2);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 4.0);
}

TEST(SymbolFunctionCeiling, AlreadyInteger) {
    CeilingFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

// ============================================================================
// Cos tests (Rust cos.rs)
// ============================================================================

TEST(SymbolFunctionCos, Name) {
    CosFunction<> func;
    EXPECT_STREQ(func.name(), "cos");
}

TEST(SymbolFunctionCos, Evaluate) {
    CosFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionCos, PiHalf) {
    CosFunction<> func;
    auto result = func.evaluate_single(std::numbers::pi / 2.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 0.0, 1e-10);
}

// ============================================================================
// First tests (Rust first.rs)
// ============================================================================

TEST(SymbolFunctionFirst, Name) {
    FirstFunction<> func;
    EXPECT_STREQ(func.name(), "first");
}

TEST(SymbolFunctionFirst, Evaluate) {
    FirstFunction<> func;
    auto result = func.evaluate({10.0, 20.0, 30.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 10.0);
}

TEST(SymbolFunctionFirst, Empty) {
    FirstFunction<> func;
    auto result = func.evaluate({});
    EXPECT_FALSE(result.has_value());
}

// ============================================================================
// Floor tests (Rust floor.rs)
// ============================================================================

TEST(SymbolFunctionFloor, Name) {
    FloorFunction<> func;
    EXPECT_STREQ(func.name(), "floor");
}

TEST(SymbolFunctionFloor, Evaluate) {
    FloorFunction<> func;
    auto result = func.evaluate_single(3.7);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionFloor, AlreadyInteger) {
    FloorFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

// ============================================================================
// IfFunction tests (Rust if_function.rs)
// ============================================================================

TEST(SymbolFunctionIfFunction, Name) {
    IfFunction<> func;
    EXPECT_STREQ(func.name(), "if_function");
}

TEST(SymbolFunctionIfFunction, ConditionTrue) {
    IfFunction<> func;
    auto result = func.evaluate({1.0, 10.0, 20.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 10.0);
}

TEST(SymbolFunctionIfFunction, ConditionFalse) {
    IfFunction<> func;
    auto result = func.evaluate({0.0, 10.0, 20.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 20.0);
}

// ============================================================================
// IfIn tests (Rust if_in.rs)
// ============================================================================

TEST(SymbolFunctionIfIn, Name) {
    IfInFunction<> func;
    EXPECT_STREQ(func.name(), "if_in");
}

TEST(SymbolFunctionIfIn, InRange) {
    IfInFunction<> func;
    auto result = func.evaluate({5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionIfIn, OutOfRange) {
    IfInFunction<> func;
    auto result = func.evaluate({15.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// IfThen tests (Rust if_then.rs)
// ============================================================================

TEST(SymbolFunctionIfThen, Name) {
    IfThenFunction<> func;
    EXPECT_STREQ(func.name(), "if_then");
}

TEST(SymbolFunctionIfThen, ConditionTrue) {
    IfThenFunction<> func;
    auto result = func.evaluate_binary(1.0, 42.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 42.0);
}

TEST(SymbolFunctionIfThen, ConditionFalse) {
    IfThenFunction<> func;
    auto result = func.evaluate_binary(0.0, 42.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Imply tests (Rust imply.rs)
// ============================================================================

TEST(SymbolFunctionImply, Name) {
    ImplyFunction<> func;
    EXPECT_STREQ(func.name(), "imply");
}

TEST(SymbolFunctionImply, TrueImpliesTrue) {
    ImplyFunction<> func;
    auto result = func.evaluate_binary(1.0, 1.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionImply, TrueImpliesFalse) {
    ImplyFunction<> func;
    auto result = func.evaluate_binary(1.0, 0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionImply, FalseImpliesAnything) {
    ImplyFunction<> func;
    auto result = func.evaluate_binary(0.0, 1.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

// ============================================================================
// InStepRange tests (Rust in_step_range.rs)
// ============================================================================

TEST(SymbolFunctionInStepRange, Name) {
    InStepRangeFunction<> func;
    EXPECT_STREQ(func.name(), "in_step_range");
}

TEST(SymbolFunctionInStepRange, InRange) {
    InStepRangeFunction<> func;
    auto result = func.evaluate({2.0, 0.0, 5.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionInStepRange, OutOfRange) {
    InStepRangeFunction<> func;
    auto result = func.evaluate({7.0, 0.0, 5.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Inequality tests (Rust inequality.rs)
// ============================================================================

TEST(SymbolFunctionInequality, Name) {
    InequalityFunction<> func;
    EXPECT_STREQ(func.name(), "inequality");
}

TEST(SymbolFunctionInequality, LessEqual) {
    InequalityFunction<> func;
    auto result = func.evaluate_binary(3.0, 5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionInequality, Greater) {
    InequalityFunction<> func;
    auto result = func.evaluate_binary(5.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Masking tests (Rust masking.rs)
// ============================================================================

TEST(SymbolFunctionMasking, Name) {
    MaskingFunction<> func;
    EXPECT_STREQ(func.name(), "masking");
}

TEST(SymbolFunctionMasking, MaskOn) {
    MaskingFunction<> func;
    auto result = func.evaluate_binary(1.0, 42.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 42.0);
}

TEST(SymbolFunctionMasking, MaskOff) {
    MaskingFunction<> func;
    auto result = func.evaluate_binary(0.0, 42.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Max tests (Rust max.rs)
// ============================================================================

TEST(SymbolFunctionMax, Name) {
    MaxFunction<> func;
    EXPECT_STREQ(func.name(), "max");
}

TEST(SymbolFunctionMax, FirstLarger) {
    MaxFunction<> func;
    auto result = func.evaluate_binary(5.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionMax, SecondLarger) {
    MaxFunction<> func;
    auto result = func.evaluate_binary(3.0, 5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

// ============================================================================
// MinMax tests (Rust min_max.rs)
// ============================================================================

TEST(SymbolFunctionMinMax, Name) {
    MinMaxFunction<> func;
    EXPECT_STREQ(func.name(), "min_max");
}

TEST(SymbolFunctionMinMax, InRange) {
    MinMaxFunction<> func;
    auto result = func.evaluate({5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionMinMax, BelowMin) {
    MinMaxFunction<> func;
    auto result = func.evaluate({-5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionMinMax, AboveMax) {
    MinMaxFunction<> func;
    auto result = func.evaluate({15.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 10.0);
}

// ============================================================================
// ModFunction tests (Rust mod_function.rs)
// ============================================================================

TEST(SymbolFunctionModFunction, Name) {
    ModFunction<> func;
    EXPECT_STREQ(func.name(), "mod_function");
}

TEST(SymbolFunctionModFunction, Evaluate) {
    ModFunction<> func;
    auto result = func.evaluate_binary(10.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionModFunction, DivideByZero) {
    ModFunction<> func;
    auto result = func.evaluate_binary(10.0, 0.0);
    EXPECT_FALSE(result.has_value());
}

// ============================================================================
// OneOf tests (Rust one_of.rs)
// ============================================================================

TEST(SymbolFunctionOneOf, Name) {
    OneOfFunction<> func;
    EXPECT_STREQ(func.name(), "one_of");
}

TEST(SymbolFunctionOneOf, ExactlyOne) {
    OneOfFunction<> func;
    auto result = func.evaluate({1.0, 0.0, 0.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionOneOf, MoreThanOne) {
    OneOfFunction<> func;
    auto result = func.evaluate({1.0, 1.0, 0.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionOneOf, None) {
    OneOfFunction<> func;
    auto result = func.evaluate({0.0, 0.0, 0.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// P0EvaluationTests tests (Rust p0_evaluation_tests.rs)
// ============================================================================

TEST(SymbolFunctionP0EvaluationTests, Name) {
    P0EvaluationTestsFunction<> func;
    EXPECT_STREQ(func.name(), "p0_evaluation_tests");
}

TEST(SymbolFunctionP0EvaluationTests, Evaluate) {
    P0EvaluationTestsFunction<> func;
    auto result = func.evaluate({42.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 42.0);
}

// ============================================================================
// Product tests (Rust product.rs)
// ============================================================================

TEST(SymbolFunctionProduct, Name) {
    ProductFunction<> func;
    EXPECT_STREQ(func.name(), "product");
}

TEST(SymbolFunctionProduct, Evaluate) {
    ProductFunction<> func;
    auto result = func.evaluate({2.0, 3.0, 4.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 24.0);
}

TEST(SymbolFunctionProduct, Single) {
    ProductFunction<> func;
    auto result = func.evaluate({5.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionProduct, Empty) {
    ProductFunction<> func;
    auto result = func.evaluate({});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

// ============================================================================
// QuadraticFunction tests (Rust quadratic_function.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticFunction, Name) {
    QuadraticFunction<> func;
    EXPECT_STREQ(func.name(), "quadratic_function");
}

TEST(SymbolFunctionQuadraticFunction, Evaluate) {
    QuadraticFunction<> func;
    // x=2, a=1, b=0, c=0 -> 2^2 = 4
    auto result = func.evaluate({2.0, 1.0, 0.0, 0.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 4.0);
}

TEST(SymbolFunctionQuadraticFunction, FullQuadratic) {
    QuadraticFunction<> func;
    // x=2, a=1, b=2, c=3 -> 4 + 4 + 3 = 11
    auto result = func.evaluate({2.0, 1.0, 2.0, 3.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 11.0);
}

// ============================================================================
// QuadraticInStepRange tests (Rust quadratic_in_step_range.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticInStepRange, Name) {
    QuadraticInStepRangeFunction<> func;
    EXPECT_STREQ(func.name(), "quadratic_in_step_range");
}

TEST(SymbolFunctionQuadraticInStepRange, InRange) {
    QuadraticInStepRangeFunction<> func;
    auto result = func.evaluate({2.0, 0.0, 5.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

// ============================================================================
// QuadraticLinear tests (Rust quadratic_linear.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticLinear, Name) {
    QuadraticLinearFunction<> func;
    EXPECT_STREQ(func.name(), "quadratic_linear");
}

TEST(SymbolFunctionQuadraticLinear, Evaluate) {
    QuadraticLinearFunction<> func;
    // x=3, a=2, b=1 -> 2*3+1 = 7
    auto result = func.evaluate({3.0, 2.0, 1.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 7.0);
}

// ============================================================================
// QuadraticMaskingRange tests (Rust quadratic_masking_range.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticMaskingRange, Name) {
    QuadraticMaskingRangeFunction<> func;
    EXPECT_STREQ(func.name(), "quadratic_masking_range");
}

TEST(SymbolFunctionQuadraticMaskingRange, MaskOnInRange) {
    QuadraticMaskingRangeFunction<> func;
    auto result = func.evaluate({5.0, 1.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionQuadraticMaskingRange, MaskOff) {
    QuadraticMaskingRangeFunction<> func;
    auto result = func.evaluate({5.0, 0.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// QuadraticMin tests (Rust quadratic_min.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticMin, Name) {
    QuadraticMinFunction<> func;
    EXPECT_STREQ(func.name(), "quadratic_min");
}

TEST(SymbolFunctionQuadraticMin, FirstSmaller) {
    QuadraticMinFunction<> func;
    auto result = func.evaluate_binary(3.0, 5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionQuadraticMin, SecondSmaller) {
    QuadraticMinFunction<> func;
    auto result = func.evaluate_binary(5.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

// ============================================================================
// Rounding tests (Rust rounding.rs)
// ============================================================================

TEST(SymbolFunctionRounding, Name) {
    RoundingFunction<> func;
    EXPECT_STREQ(func.name(), "rounding");
}

TEST(SymbolFunctionRounding, RoundUp) {
    RoundingFunction<> func;
    auto result = func.evaluate_single(3.6);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 4.0);
}

TEST(SymbolFunctionRounding, RoundDown) {
    RoundingFunction<> func;
    auto result = func.evaluate_single(3.4);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionRounding, AlreadyInteger) {
    RoundingFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

// ============================================================================
// SameAs tests (Rust same_as.rs)
// ============================================================================

TEST(SymbolFunctionSameAs, Name) {
    SameAsFunction<> func;
    EXPECT_STREQ(func.name(), "same_as");
}

TEST(SymbolFunctionSameAs, Equal) {
    SameAsFunction<> func;
    auto result = func.evaluate_binary(5.0, 5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

TEST(SymbolFunctionSameAs, NotEqual) {
    SameAsFunction<> func;
    auto result = func.evaluate_binary(5.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// SatisfiedAmount tests (Rust satisfied_amount.rs)
// ============================================================================

TEST(SymbolFunctionSatisfiedAmount, Name) {
    SatisfiedAmountFunction<> func;
    EXPECT_STREQ(func.name(), "satisfied_amount");
}

TEST(SymbolFunctionSatisfiedAmount, AllSatisfied) {
    SatisfiedAmountFunction<> func;
    auto result = func.evaluate({1.0, 1.0, 1.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionSatisfiedAmount, NoneSatisfied) {
    SatisfiedAmountFunction<> func;
    auto result = func.evaluate({0.0, 0.0, 0.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSatisfiedAmount, Partial) {
    SatisfiedAmountFunction<> func;
    auto result = func.evaluate({1.0, 0.0, 1.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 2.0);
}

// ============================================================================
// SatisfiedAmountInequality tests (Rust satisfied_amount_inequality.rs)
// ============================================================================

TEST(SymbolFunctionSatisfiedAmountInequality, Name) {
    SatisfiedAmountInequalityFunction<> func;
    EXPECT_STREQ(func.name(), "satisfied_amount_inequality");
}

TEST(SymbolFunctionSatisfiedAmountInequality, AllSatisfied) {
    SatisfiedAmountInequalityFunction<> func;
    // pairs: (1,5), (2,6), (3,7) -> all satisfied
    auto result = func.evaluate({1.0, 5.0, 2.0, 6.0, 3.0, 7.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.0);
}

TEST(SymbolFunctionSatisfiedAmountInequality, NoneSatisfied) {
    SatisfiedAmountInequalityFunction<> func;
    // pairs: (5,1), (6,2) -> none satisfied
    auto result = func.evaluate({5.0, 1.0, 6.0, 2.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// Semantic tests (Rust semantic.rs)
// ============================================================================

TEST(SymbolFunctionSemantic, Name) {
    SemanticFunction<> func;
    EXPECT_STREQ(func.name(), "semantic");
}

TEST(SymbolFunctionSemantic, Evaluate) {
    SemanticFunction<> func;
    auto result = func.evaluate({42.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 42.0);
}

// ============================================================================
// Semi tests (Rust semi.rs)
// ============================================================================

TEST(SymbolFunctionSemi, Name) {
    SemiFunction<> func;
    EXPECT_STREQ(func.name(), "semi");
}

TEST(SymbolFunctionSemi, IndicatorOn) {
    SemiFunction<> func;
    // indicator=1, value=5, lower=0, upper=10 -> 5
    auto result = func.evaluate({1.0, 5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionSemi, IndicatorOff) {
    SemiFunction<> func;
    // indicator=0, value=5, lower=0, upper=10 -> 0
    auto result = func.evaluate({0.0, 5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSemi, ClampToLower) {
    SemiFunction<> func;
    // indicator=1, value=-5, lower=0, upper=10 -> 0
    auto result = func.evaluate({1.0, -5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSemi, ClampToUpper) {
    SemiFunction<> func;
    // indicator=1, value=15, lower=0, upper=10 -> 10
    auto result = func.evaluate({1.0, 15.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 10.0);
}

// ============================================================================
// Sigmoid tests (Rust sigmoid.rs)
// ============================================================================

TEST(SymbolFunctionSigmoid, Name) {
    SigmoidFunction<> func;
    EXPECT_STREQ(func.name(), "sigmoid");
}

TEST(SymbolFunctionSigmoid, Zero) {
    SigmoidFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.5);
}

TEST(SymbolFunctionSigmoid, LargePositive) {
    SigmoidFunction<> func;
    auto result = func.evaluate_single(100.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 1.0, 1e-10);
}

TEST(SymbolFunctionSigmoid, LargeNegative) {
    SigmoidFunction<> func;
    auto result = func.evaluate_single(-100.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 0.0, 1e-10);
}

// ============================================================================
// Sin tests (Rust sin.rs)
// ============================================================================

TEST(SymbolFunctionSin, Name) {
    SinFunction<> func;
    EXPECT_STREQ(func.name(), "sin");
}

TEST(SymbolFunctionSin, Zero) {
    SinFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSin, PiHalf) {
    SinFunction<> func;
    auto result = func.evaluate_single(std::numbers::pi / 2.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(*result, 1.0, 1e-10);
}

// ============================================================================
// Slack tests (Rust slack.rs)
// ============================================================================

TEST(SymbolFunctionSlack, Name) {
    SlackFunction<> func;
    EXPECT_STREQ(func.name(), "slack");
}

TEST(SymbolFunctionSlack, Positive) {
    SlackFunction<> func;
    auto result = func.evaluate_single(5.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

TEST(SymbolFunctionSlack, Negative) {
    SlackFunction<> func;
    auto result = func.evaluate_single(-3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSlack, Zero) {
    SlackFunction<> func;
    auto result = func.evaluate_single(0.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

// ============================================================================
// SlackRange tests (Rust slack_range.rs)
// ============================================================================

TEST(SymbolFunctionSlackRange, Name) {
    SlackRangeFunction<> func;
    EXPECT_STREQ(func.name(), "slack_range");
}

TEST(SymbolFunctionSlackRange, InRange) {
    SlackRangeFunction<> func;
    auto result = func.evaluate({5.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 0.0);
}

TEST(SymbolFunctionSlackRange, BelowLower) {
    SlackRangeFunction<> func;
    auto result = func.evaluate({-2.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 2.0);
}

TEST(SymbolFunctionSlackRange, AboveUpper) {
    SlackRangeFunction<> func;
    auto result = func.evaluate({15.0, 0.0, 10.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 5.0);
}

// ============================================================================
// UnivariateLinearPiecewise tests (Rust univariate_linear_piecewise.rs)
// ============================================================================

TEST(SymbolFunctionUnivariateLinearPiecewise, Name) {
    UnivariateLinearPiecewiseFunction<> func;
    EXPECT_STREQ(func.name(), "univariate_linear_piecewise");
}

TEST(SymbolFunctionUnivariateLinearPiecewise, Evaluate) {
    UnivariateLinearPiecewiseFunction<> func;
    // x=3, slope=2, intercept=1 -> 2*3+1 = 7
    auto result = func.evaluate({3.0, 2.0, 1.0});
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 7.0);
}

// ============================================================================
// Mod tests (Rust mod.rs)
// ============================================================================

TEST(SymbolFunctionMod, Name) {
    ModFunctionSimple<double> func;
    EXPECT_STREQ(func.name(), "mod");
}

TEST(SymbolFunctionMod, Evaluate) {
    ModFunctionSimple<double> func;
    auto result = func.evaluate_binary(10.0, 3.0);
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 1.0);
}

// ============================================================================
// Cross-function integration tests
// ============================================================================

TEST(SymbolFunctionsIntegration, AllFunctionsHaveName) {
    AbsFunction<> a; EXPECT_STREQ(a.name(), "abs");
    AndFunction<> b; EXPECT_STREQ(b.name(), "and");
    BalanceTernaryzationFunction<> c; EXPECT_STREQ(c.name(), "balance_ternaryzation");
    BigMFunction<> d; EXPECT_STREQ(d.name(), "big_m");
    BinaryzationFunction<> e; EXPECT_STREQ(e.name(), "binaryzation");
    CeilingFunction<> g; EXPECT_STREQ(g.name(), "ceiling");
    CosFunction<> h; EXPECT_STREQ(h.name(), "cos");
    FirstFunction<> i; EXPECT_STREQ(i.name(), "first");
    FloorFunction<> j; EXPECT_STREQ(j.name(), "floor");
    IfFunction<> k; EXPECT_STREQ(k.name(), "if_function");
    IfInFunction<> l; EXPECT_STREQ(l.name(), "if_in");
    IfThenFunction<> m; EXPECT_STREQ(m.name(), "if_then");
    ImplyFunction<> n; EXPECT_STREQ(n.name(), "imply");
    InequalityFunction<> p; EXPECT_STREQ(p.name(), "inequality");
    MaskingFunction<> q; EXPECT_STREQ(q.name(), "masking");
    MaxFunction<> r; EXPECT_STREQ(r.name(), "max");
    MinMaxFunction<> s; EXPECT_STREQ(s.name(), "min_max");
    ModFunction<> t; EXPECT_STREQ(t.name(), "mod_function");
    OneOfFunction<> u; EXPECT_STREQ(u.name(), "one_of");
    ProductFunction<> w; EXPECT_STREQ(w.name(), "product");
    QuadraticFunction<> x; EXPECT_STREQ(x.name(), "quadratic_function");
    QuadraticMinFunction<> ab; EXPECT_STREQ(ab.name(), "quadratic_min");
    RoundingFunction<> ac; EXPECT_STREQ(ac.name(), "rounding");
    SameAsFunction<> ad; EXPECT_STREQ(ad.name(), "same_as");
    SatisfiedAmountFunction<> ae; EXPECT_STREQ(ae.name(), "satisfied_amount");
    SemanticFunction<> ag; EXPECT_STREQ(ag.name(), "semantic");
    SemiFunction<> ah; EXPECT_STREQ(ah.name(), "semi");
    SigmoidFunction<> ai; EXPECT_STREQ(ai.name(), "sigmoid");
    SinFunction<> aj; EXPECT_STREQ(aj.name(), "sin");
    SlackFunction<> ak; EXPECT_STREQ(ak.name(), "slack");
    UnivariateLinearPiecewiseFunction<> am; EXPECT_STREQ(am.name(), "univariate_linear_piecewise");
}

TEST(SymbolFunctionsIntegration, AllFunctionsInstantiable) {
    AbsFunction<> a; AndFunction<> b; BalanceTernaryzationFunction<> c;
    BigMFunction<> d; BinaryzationFunction<> e; BivariateLinearPiecewiseFunction<> f;
    CeilingFunction<> g; CosFunction<> h; FirstFunction<> i;
    FloorFunction<> j; IfFunction<> k; IfInFunction<> l;
    IfThenFunction<> m; ImplyFunction<> n; InStepRangeFunction<> o;
    InequalityFunction<> p; MaskingFunction<> q; MaxFunction<> r;
    MinMaxFunction<> s; ModFunction<> t; OneOfFunction<> u;
    P0EvaluationTestsFunction<> v; ProductFunction<> w; QuadraticFunction<> x;
    QuadraticInStepRangeFunction<> y; QuadraticLinearFunction<> z;
    QuadraticMaskingRangeFunction<> aa; QuadraticMinFunction<> ab;
    RoundingFunction<> ac; SameAsFunction<> ad; SatisfiedAmountFunction<> ae;
    SatisfiedAmountInequalityFunction<> af; SemanticFunction<> ag;
    SemiFunction<> ah; SigmoidFunction<> ai; SinFunction<> aj;
    SlackFunction<> ak; SlackRangeFunction<> al;
    UnivariateLinearPiecewiseFunction<> am; ModFunctionSimple<double> an;
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    (void)g; (void)h; (void)i; (void)j; (void)k; (void)l;
    (void)m; (void)n; (void)o; (void)p; (void)q; (void)r;
    (void)s; (void)t; (void)u; (void)v; (void)w; (void)x;
    (void)y; (void)z; (void)aa; (void)ab; (void)ac; (void)ad;
    (void)ae; (void)af; (void)ag; (void)ah; (void)ai; (void)aj;
    (void)ak; (void)al; (void)am; (void)an;
}
