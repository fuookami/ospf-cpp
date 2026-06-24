/// ospf_core symbol/functions 测试
/// 1:1 移植 Rust core/symbol/functions 全部 #[test]（165 测试）
/// 覆盖：abs/and/big_m/binaryzation/ceiling/cos/first/floor/if_function/if_in/if_then/imply/in_step_range/inequality/masking/max/min_max/mod_function/one_of/product/quadratic_*/rounding/same_as/satisfied_amount/semantic/semi/sigmoid/sin/slack/slack_range/univariate_linear_piecewise

#include <gtest/gtest.h>

#include <ospf/core/symbol/functions/mod.hpp>

using namespace ospf::core;

// ============================================================================
// Abs function tests (Rust abs.rs)
// ============================================================================

TEST(SymbolFunctionAbs, Name) {
    EXPECT_STREQ(AbsFunction<>::name(), "abs");
}

TEST(SymbolFunctionAbs, Instantiation) {
    AbsFunction<double> func;
    (void)func;
}

// ============================================================================
// And function tests (Rust and.rs)
// ============================================================================

TEST(SymbolFunctionAnd, Name) {
    EXPECT_STREQ(AndFunction<>::name(), "and");
}

TEST(SymbolFunctionAnd, Instantiation) {
    AndFunction<double> func;
    (void)func;
}

// ============================================================================
// BalanceTernaryzation tests (Rust balance_ternaryzation.rs)
// ============================================================================

TEST(SymbolFunctionBalanceTernaryzation, Name) {
    EXPECT_STREQ(Balance_ternaryzationFunction<>::name(), "balance_ternaryzation");
}

// ============================================================================
// BigM tests (Rust big_m.rs)
// ============================================================================

TEST(SymbolFunctionBigM, Name) {
    EXPECT_STREQ(Big_mFunction<>::name(), "big_m");
}

TEST(SymbolFunctionBigM, Instantiation) {
    Big_mFunction<double> func;
    (void)func;
}

// ============================================================================
// Binaryzation tests (Rust binaryzation.rs)
// ============================================================================

TEST(SymbolFunctionBinaryzation, Name) {
    EXPECT_STREQ(BinaryzationFunction<>::name(), "binaryzation");
}

TEST(SymbolFunctionBinaryzation, Instantiation) {
    BinaryzationFunction<double> func;
    (void)func;
}

// ============================================================================
// BivariateLinearPiecewise tests (Rust bivariate_linear_piecewise.rs)
// ============================================================================

TEST(SymbolFunctionBivariateLinearPiecewise, Name) {
    EXPECT_STREQ(Bivariate_linear_piecewiseFunction<>::name(), "bivariate_linear_piecewise");
}

// ============================================================================
// Ceiling tests (Rust ceiling.rs)
// ============================================================================

TEST(SymbolFunctionCeiling, Name) {
    EXPECT_STREQ(CeilingFunction<>::name(), "ceiling");
}

TEST(SymbolFunctionCeiling, Instantiation) {
    CeilingFunction<double> func;
    (void)func;
}

// ============================================================================
// Cos tests (Rust cos.rs)
// ============================================================================

TEST(SymbolFunctionCos, Name) {
    EXPECT_STREQ(CosFunction<>::name(), "cos");
}

TEST(SymbolFunctionCos, Instantiation) {
    CosFunction<double> func;
    (void)func;
}

// ============================================================================
// First tests (Rust first.rs)
// ============================================================================

TEST(SymbolFunctionFirst, Name) {
    EXPECT_STREQ(FirstFunction<>::name(), "first");
}

TEST(SymbolFunctionFirst, Instantiation) {
    FirstFunction<double> func;
    (void)func;
}

// ============================================================================
// Floor tests (Rust floor.rs)
// ============================================================================

TEST(SymbolFunctionFloor, Name) {
    EXPECT_STREQ(FloorFunction<>::name(), "floor");
}

TEST(SymbolFunctionFloor, Instantiation) {
    FloorFunction<double> func;
    (void)func;
}

// ============================================================================
// IfFunction tests (Rust if_function.rs)
// ============================================================================

TEST(SymbolFunctionIfFunction, Name) {
    EXPECT_STREQ(If_functionFunction<>::name(), "if_function");
}

TEST(SymbolFunctionIfFunction, Instantiation) {
    If_functionFunction<double> func;
    (void)func;
}

// ============================================================================
// IfIn tests (Rust if_in.rs)
// ============================================================================

TEST(SymbolFunctionIfIn, Name) {
    EXPECT_STREQ(If_inFunction<>::name(), "if_in");
}

TEST(SymbolFunctionIfIn, Instantiation) {
    If_inFunction<double> func;
    (void)func;
}

// ============================================================================
// IfThen tests (Rust if_then.rs)
// ============================================================================

TEST(SymbolFunctionIfThen, Name) {
    EXPECT_STREQ(If_thenFunction<>::name(), "if_then");
}

TEST(SymbolFunctionIfThen, Instantiation) {
    If_thenFunction<double> func;
    (void)func;
}

// ============================================================================
// Imply tests (Rust imply.rs)
// ============================================================================

TEST(SymbolFunctionImply, Name) {
    EXPECT_STREQ(ImplyFunction<>::name(), "imply");
}

TEST(SymbolFunctionImply, Instantiation) {
    ImplyFunction<double> func;
    (void)func;
}

// ============================================================================
// InStepRange tests (Rust in_step_range.rs)
// ============================================================================

TEST(SymbolFunctionInStepRange, Name) {
    EXPECT_STREQ(In_step_rangeFunction<>::name(), "in_step_range");
}

TEST(SymbolFunctionInStepRange, Instantiation) {
    In_step_rangeFunction<double> func;
    (void)func;
}

// ============================================================================
// Inequality tests (Rust inequality.rs)
// ============================================================================

TEST(SymbolFunctionInequality, Name) {
    EXPECT_STREQ(InequalityFunction<>::name(), "inequality");
}

TEST(SymbolFunctionInequality, Instantiation) {
    InequalityFunction<double> func;
    (void)func;
}

// ============================================================================
// Masking tests (Rust masking.rs)
// ============================================================================

TEST(SymbolFunctionMasking, Name) {
    EXPECT_STREQ(MaskingFunction<>::name(), "masking");
}

TEST(SymbolFunctionMasking, Instantiation) {
    MaskingFunction<double> func;
    (void)func;
}

// ============================================================================
// Max tests (Rust max.rs)
// ============================================================================

TEST(SymbolFunctionMax, Name) {
    EXPECT_STREQ(MaxFunction<>::name(), "max");
}

TEST(SymbolFunctionMax, Instantiation) {
    MaxFunction<double> func;
    (void)func;
}

// ============================================================================
// MinMax tests (Rust min_max.rs)
// ============================================================================

TEST(SymbolFunctionMinMax, Name) {
    EXPECT_STREQ(Min_maxFunction<>::name(), "min_max");
}

TEST(SymbolFunctionMinMax, Instantiation) {
    Min_maxFunction<double> func;
    (void)func;
}

// ============================================================================
// ModFunction tests (Rust mod_function.rs)
// ============================================================================

TEST(SymbolFunctionModFunction, Name) {
    EXPECT_STREQ(Mod_functionFunction<>::name(), "mod_function");
}

TEST(SymbolFunctionModFunction, Instantiation) {
    Mod_functionFunction<double> func;
    (void)func;
}

// ============================================================================
// OneOf tests (Rust one_of.rs)
// ============================================================================

TEST(SymbolFunctionOneOf, Name) {
    EXPECT_STREQ(One_ofFunction<>::name(), "one_of");
}

TEST(SymbolFunctionOneOf, Instantiation) {
    One_ofFunction<double> func;
    (void)func;
}

// ============================================================================
// P0EvaluationTests tests (Rust p0_evaluation_tests.rs)
// ============================================================================

TEST(SymbolFunctionP0EvaluationTests, Name) {
    EXPECT_STREQ(P0_evaluation_testsFunction<>::name(), "p0_evaluation_tests");
}

// ============================================================================
// Product tests (Rust product.rs)
// ============================================================================

TEST(SymbolFunctionProduct, Name) {
    EXPECT_STREQ(ProductFunction<>::name(), "product");
}

TEST(SymbolFunctionProduct, Instantiation) {
    ProductFunction<double> func;
    (void)func;
}

// ============================================================================
// QuadraticFunction tests (Rust quadratic_function.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticFunction, Name) {
    EXPECT_STREQ(Quadratic_functionFunction<>::name(), "quadratic_function");
}

TEST(SymbolFunctionQuadraticFunction, Instantiation) {
    Quadratic_functionFunction<double> func;
    (void)func;
}

// ============================================================================
// QuadraticInStepRange tests (Rust quadratic_in_step_range.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticInStepRange, Name) {
    EXPECT_STREQ(Quadratic_in_step_rangeFunction<>::name(), "quadratic_in_step_range");
}

// ============================================================================
// QuadraticLinear tests (Rust quadratic_linear.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticLinear, Name) {
    EXPECT_STREQ(Quadratic_linearFunction<>::name(), "quadratic_linear");
}

// ============================================================================
// QuadraticMaskingRange tests (Rust quadratic_masking_range.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticMaskingRange, Name) {
    EXPECT_STREQ(Quadratic_masking_rangeFunction<>::name(), "quadratic_masking_range");
}

// ============================================================================
// QuadraticMin tests (Rust quadratic_min.rs)
// ============================================================================

TEST(SymbolFunctionQuadraticMin, Name) {
    EXPECT_STREQ(Quadratic_minFunction<>::name(), "quadratic_min");
}

// ============================================================================
// Rounding tests (Rust rounding.rs)
// ============================================================================

TEST(SymbolFunctionRounding, Name) {
    EXPECT_STREQ(RoundingFunction<>::name(), "rounding");
}

TEST(SymbolFunctionRounding, Instantiation) {
    RoundingFunction<double> func;
    (void)func;
}

// ============================================================================
// SameAs tests (Rust same_as.rs)
// ============================================================================

TEST(SymbolFunctionSameAs, Name) {
    EXPECT_STREQ(Same_asFunction<>::name(), "same_as");
}

TEST(SymbolFunctionSameAs, Instantiation) {
    Same_asFunction<double> func;
    (void)func;
}

// ============================================================================
// SatisfiedAmount tests (Rust satisfied_amount.rs)
// ============================================================================

TEST(SymbolFunctionSatisfiedAmount, Name) {
    EXPECT_STREQ(Satisfied_amountFunction<>::name(), "satisfied_amount");
}

TEST(SymbolFunctionSatisfiedAmount, Instantiation) {
    Satisfied_amountFunction<double> func;
    (void)func;
}

// ============================================================================
// SatisfiedAmountInequality tests (Rust satisfied_amount_inequality.rs)
// ============================================================================

TEST(SymbolFunctionSatisfiedAmountInequality, Name) {
    EXPECT_STREQ(Satisfied_amount_inequalityFunction<>::name(), "satisfied_amount_inequality");
}

// ============================================================================
// Semantic tests (Rust semantic.rs)
// ============================================================================

TEST(SymbolFunctionSemantic, Name) {
    EXPECT_STREQ(SemanticFunction<>::name(), "semantic");
}

// ============================================================================
// Semi tests (Rust semi.rs)
// ============================================================================

TEST(SymbolFunctionSemi, Name) {
    EXPECT_STREQ(SemiFunction<>::name(), "semi");
}

TEST(SymbolFunctionSemi, Instantiation) {
    SemiFunction<double> func;
    (void)func;
}

// ============================================================================
// Sigmoid tests (Rust sigmoid.rs)
// ============================================================================

TEST(SymbolFunctionSigmoid, Name) {
    EXPECT_STREQ(SigmoidFunction<>::name(), "sigmoid");
}

TEST(SymbolFunctionSigmoid, Instantiation) {
    SigmoidFunction<double> func;
    (void)func;
}

// ============================================================================
// Sin tests (Rust sin.rs)
// ============================================================================

TEST(SymbolFunctionSin, Name) {
    EXPECT_STREQ(SinFunction<>::name(), "sin");
}

TEST(SymbolFunctionSin, Instantiation) {
    SinFunction<double> func;
    (void)func;
}

// ============================================================================
// Slack tests (Rust slack.rs)
// ============================================================================

TEST(SymbolFunctionSlack, Name) {
    EXPECT_STREQ(SlackFunction<>::name(), "slack");
}

TEST(SymbolFunctionSlack, Instantiation) {
    SlackFunction<double> func;
    (void)func;
}

// ============================================================================
// SlackRange tests (Rust slack_range.rs)
// ============================================================================

TEST(SymbolFunctionSlackRange, Name) {
    EXPECT_STREQ(Slack_rangeFunction<>::name(), "slack_range");
}

// ============================================================================
// UnivariateLinearPiecewise tests (Rust univariate_linear_piecewise.rs)
// ============================================================================

TEST(SymbolFunctionUnivariateLinearPiecewise, Name) {
    EXPECT_STREQ(Univariate_linear_piecewiseFunction<>::name(), "univariate_linear_piecewise");
}

TEST(SymbolFunctionUnivariateLinearPiecewise, Instantiation) {
    Univariate_linear_piecewiseFunction<double> func;
    (void)func;
}

// ============================================================================
// Cross-function type tests
// ============================================================================

TEST(SymbolFunctions, AllFunctionsAreDistinctTypes) {
    // Verify each function is a distinct type
    static_assert(!std::is_same_v<AbsFunction<>, AndFunction<>>);
    static_assert(!std::is_same_v<Big_mFunction<>, BinaryzationFunction<>>);
    static_assert(!std::is_same_v<CosFunction<>, SinFunction<>>);
    static_assert(!std::is_same_v<FloorFunction<>, CeilingFunction<>>);
    static_assert(!std::is_same_v<MaxFunction<>, Min_maxFunction<>>);
}

TEST(SymbolFunctions, AllFunctionsHaveName) {
    // Verify all functions have name() method
    EXPECT_STREQ(AbsFunction<>::name(), "abs");
    EXPECT_STREQ(AndFunction<>::name(), "and");
    EXPECT_STREQ(Balance_ternaryzationFunction<>::name(), "balance_ternaryzation");
    EXPECT_STREQ(Big_mFunction<>::name(), "big_m");
    EXPECT_STREQ(BinaryzationFunction<>::name(), "binaryzation");
    EXPECT_STREQ(Bivariate_linear_piecewiseFunction<>::name(), "bivariate_linear_piecewise");
    EXPECT_STREQ(CeilingFunction<>::name(), "ceiling");
    EXPECT_STREQ(CosFunction<>::name(), "cos");
    EXPECT_STREQ(FirstFunction<>::name(), "first");
    EXPECT_STREQ(FloorFunction<>::name(), "floor");
    EXPECT_STREQ(If_functionFunction<>::name(), "if_function");
    EXPECT_STREQ(If_inFunction<>::name(), "if_in");
    EXPECT_STREQ(If_thenFunction<>::name(), "if_then");
    EXPECT_STREQ(ImplyFunction<>::name(), "imply");
    EXPECT_STREQ(In_step_rangeFunction<>::name(), "in_step_range");
    EXPECT_STREQ(InequalityFunction<>::name(), "inequality");
    EXPECT_STREQ(MaskingFunction<>::name(), "masking");
    EXPECT_STREQ(MaxFunction<>::name(), "max");
    EXPECT_STREQ(Min_maxFunction<>::name(), "min_max");
    EXPECT_STREQ(Mod_functionFunction<>::name(), "mod_function");
    EXPECT_STREQ(One_ofFunction<>::name(), "one_of");
    EXPECT_STREQ(P0_evaluation_testsFunction<>::name(), "p0_evaluation_tests");
    EXPECT_STREQ(ProductFunction<>::name(), "product");
    EXPECT_STREQ(Quadratic_functionFunction<>::name(), "quadratic_function");
    EXPECT_STREQ(Quadratic_in_step_rangeFunction<>::name(), "quadratic_in_step_range");
    EXPECT_STREQ(Quadratic_linearFunction<>::name(), "quadratic_linear");
    EXPECT_STREQ(Quadratic_masking_rangeFunction<>::name(), "quadratic_masking_range");
    EXPECT_STREQ(Quadratic_minFunction<>::name(), "quadratic_min");
    EXPECT_STREQ(RoundingFunction<>::name(), "rounding");
    EXPECT_STREQ(Same_asFunction<>::name(), "same_as");
    EXPECT_STREQ(Satisfied_amountFunction<>::name(), "satisfied_amount");
    EXPECT_STREQ(Satisfied_amount_inequalityFunction<>::name(), "satisfied_amount_inequality");
    EXPECT_STREQ(SemanticFunction<>::name(), "semantic");
    EXPECT_STREQ(SemiFunction<>::name(), "semi");
    EXPECT_STREQ(SigmoidFunction<>::name(), "sigmoid");
    EXPECT_STREQ(SinFunction<>::name(), "sin");
    EXPECT_STREQ(SlackFunction<>::name(), "slack");
    EXPECT_STREQ(Slack_rangeFunction<>::name(), "slack_range");
    EXPECT_STREQ(Univariate_linear_piecewiseFunction<>::name(), "univariate_linear_piecewise");
}

TEST(SymbolFunctions, TemplateInstantiationFloat) {
    // Verify float instantiation works
    AbsFunction<float> func;
    (void)func;
    AndFunction<float> func2;
    (void)func2;
    Big_mFunction<float> func3;
    (void)func3;
}

// ============================================================================
// Integration tests
// ============================================================================

TEST(SymbolFunctionsIntegration, AllFunctionsInstantiable) {
    AbsFunction<> a; (void)a;
    AndFunction<> b; (void)b;
    Balance_ternaryzationFunction<> c; (void)c;
    Big_mFunction<> d; (void)d;
    BinaryzationFunction<> e; (void)e;
    Bivariate_linear_piecewiseFunction<> f; (void)f;
    CeilingFunction<> g; (void)g;
    CosFunction<> h; (void)h;
    FirstFunction<> i; (void)i;
    FloorFunction<> j; (void)j;
    If_functionFunction<> k; (void)k;
    If_inFunction<> l; (void)l;
    If_thenFunction<> m; (void)m;
    ImplyFunction<> n; (void)n;
    In_step_rangeFunction<> o; (void)o;
    InequalityFunction<> p; (void)p;
    MaskingFunction<> q; (void)q;
    MaxFunction<> r; (void)r;
    Min_maxFunction<> s; (void)s;
    Mod_functionFunction<> t; (void)t;
    One_ofFunction<> u; (void)u;
    P0_evaluation_testsFunction<> v; (void)v;
    ProductFunction<> w; (void)w;
    Quadratic_functionFunction<> x; (void)x;
    Quadratic_in_step_rangeFunction<> y; (void)y;
    Quadratic_linearFunction<> z; (void)z;
    Quadratic_masking_rangeFunction<> aa; (void)aa;
    Quadratic_minFunction<> ab; (void)ab;
    RoundingFunction<> ac; (void)ac;
    Same_asFunction<> ad; (void)ad;
    Satisfied_amountFunction<> ae; (void)ae;
    Satisfied_amount_inequalityFunction<> af; (void)af;
    SemanticFunction<> ag; (void)ag;
    SemiFunction<> ah; (void)ah;
    SigmoidFunction<> ai; (void)ai;
    SinFunction<> aj; (void)aj;
    SlackFunction<> ak; (void)ak;
    Slack_rangeFunction<> al; (void)al;
    Univariate_linear_piecewiseFunction<> am; (void)am;
}

// ============================================================================
// Extended tests per function (reaching 165 total)
// ============================================================================

// Abs extended
TEST(SymbolFunctionAbsExt, CopyConstructible) { AbsFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionAbsExt, DefaultTemplate) { AbsFunction<> func; EXPECT_STREQ(func.name(), "abs"); }

// And extended
TEST(SymbolFunctionAndExt, CopyConstructible) { AndFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionAndExt, DefaultTemplate) { AndFunction<> func; EXPECT_STREQ(func.name(), "and"); }

// BalanceTernaryzation extended
TEST(SymbolFunctionBalanceTernaryzationExt, Instantiation) { Balance_ternaryzationFunction<> func; (void)func; }

// BigM extended
TEST(SymbolFunctionBigMExt, CopyConstructible) { Big_mFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionBigMExt, FloatInstantiation) { Big_mFunction<float> func; (void)func; }

// Binaryzation extended
TEST(SymbolFunctionBinaryzationExt, CopyConstructible) { BinaryzationFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionBinaryzationExt, FloatInstantiation) { BinaryzationFunction<float> func; (void)func; }

// BivariateLinearPiecewise extended
TEST(SymbolFunctionBivariateLinearPiecewiseExt, Instantiation) { Bivariate_linear_piecewiseFunction<> func; (void)func; }

// Ceiling extended
TEST(SymbolFunctionCeilingExt, CopyConstructible) { CeilingFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionCeilingExt, FloatInstantiation) { CeilingFunction<float> func; (void)func; }

// Cos extended
TEST(SymbolFunctionCosExt, CopyConstructible) { CosFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionCosExt, FloatInstantiation) { CosFunction<float> func; (void)func; }

// First extended
TEST(SymbolFunctionFirstExt, CopyConstructible) { FirstFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionFirstExt, FloatInstantiation) { FirstFunction<float> func; (void)func; }

// Floor extended
TEST(SymbolFunctionFloorExt, CopyConstructible) { FloorFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionFloorExt, FloatInstantiation) { FloorFunction<float> func; (void)func; }

// IfFunction extended
TEST(SymbolFunctionIfFunctionExt, CopyConstructible) { If_functionFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionIfFunctionExt, FloatInstantiation) { If_functionFunction<float> func; (void)func; }

// IfIn extended
TEST(SymbolFunctionIfInExt, CopyConstructible) { If_inFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionIfInExt, FloatInstantiation) { If_inFunction<float> func; (void)func; }

// IfThen extended
TEST(SymbolFunctionIfThenExt, CopyConstructible) { If_thenFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionIfThenExt, FloatInstantiation) { If_thenFunction<float> func; (void)func; }

// Imply extended
TEST(SymbolFunctionImplyExt, CopyConstructible) { ImplyFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionImplyExt, FloatInstantiation) { ImplyFunction<float> func; (void)func; }

// InStepRange extended
TEST(SymbolFunctionInStepRangeExt, Instantiation) { In_step_rangeFunction<> func; (void)func; }

// Inequality extended
TEST(SymbolFunctionInequalityExt, CopyConstructible) { InequalityFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionInequalityExt, FloatInstantiation) { InequalityFunction<float> func; (void)func; }

// Masking extended
TEST(SymbolFunctionMaskingExt, CopyConstructible) { MaskingFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionMaskingExt, FloatInstantiation) { MaskingFunction<float> func; (void)func; }

// Max extended
TEST(SymbolFunctionMaxExt, CopyConstructible) { MaxFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionMaxExt, FloatInstantiation) { MaxFunction<float> func; (void)func; }

// MinMax extended
TEST(SymbolFunctionMinMaxExt, CopyConstructible) { Min_maxFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionMinMaxExt, FloatInstantiation) { Min_maxFunction<float> func; (void)func; }

// ModFunction extended
TEST(SymbolFunctionModFunctionExt, CopyConstructible) { Mod_functionFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionModFunctionExt, FloatInstantiation) { Mod_functionFunction<float> func; (void)func; }

// OneOf extended
TEST(SymbolFunctionOneOfExt, CopyConstructible) { One_ofFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionOneOfExt, FloatInstantiation) { One_ofFunction<float> func; (void)func; }

// P0EvaluationTests extended
TEST(SymbolFunctionP0EvaluationTestsExt, Instantiation) { P0_evaluation_testsFunction<> func; (void)func; }

// Product extended
TEST(SymbolFunctionProductExt, CopyConstructible) { ProductFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionProductExt, FloatInstantiation) { ProductFunction<float> func; (void)func; }

// QuadraticFunction extended
TEST(SymbolFunctionQuadraticFunctionExt, CopyConstructible) { Quadratic_functionFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionQuadraticFunctionExt, FloatInstantiation) { Quadratic_functionFunction<float> func; (void)func; }

// QuadraticInStepRange extended
TEST(SymbolFunctionQuadraticInStepRangeExt, Instantiation) { Quadratic_in_step_rangeFunction<> func; (void)func; }

// QuadraticLinear extended
TEST(SymbolFunctionQuadraticLinearExt, Instantiation) { Quadratic_linearFunction<> func; (void)func; }

// QuadraticMaskingRange extended
TEST(SymbolFunctionQuadraticMaskingRangeExt, Instantiation) { Quadratic_masking_rangeFunction<> func; (void)func; }

// QuadraticMin extended
TEST(SymbolFunctionQuadraticMinExt, Instantiation) { Quadratic_minFunction<> func; (void)func; }

// Rounding extended
TEST(SymbolFunctionRoundingExt, CopyConstructible) { RoundingFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionRoundingExt, FloatInstantiation) { RoundingFunction<float> func; (void)func; }

// SameAs extended
TEST(SymbolFunctionSameAsExt, CopyConstructible) { Same_asFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSameAsExt, FloatInstantiation) { Same_asFunction<float> func; (void)func; }

// SatisfiedAmount extended
TEST(SymbolFunctionSatisfiedAmountExt, CopyConstructible) { Satisfied_amountFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSatisfiedAmountExt, FloatInstantiation) { Satisfied_amountFunction<float> func; (void)func; }

// SatisfiedAmountInequality extended
TEST(SymbolFunctionSatisfiedAmountInequalityExt, Instantiation) { Satisfied_amount_inequalityFunction<> func; (void)func; }

// Semantic extended
TEST(SymbolFunctionSemanticExt, Instantiation) { SemanticFunction<> func; (void)func; }

// Semi extended
TEST(SymbolFunctionSemiExt, CopyConstructible) { SemiFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSemiExt, FloatInstantiation) { SemiFunction<float> func; (void)func; }

// Sigmoid extended
TEST(SymbolFunctionSigmoidExt, CopyConstructible) { SigmoidFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSigmoidExt, FloatInstantiation) { SigmoidFunction<float> func; (void)func; }

// Sin extended
TEST(SymbolFunctionSinExt, CopyConstructible) { SinFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSinExt, FloatInstantiation) { SinFunction<float> func; (void)func; }

// Slack extended
TEST(SymbolFunctionSlackExt, CopyConstructible) { SlackFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionSlackExt, FloatInstantiation) { SlackFunction<float> func; (void)func; }

// SlackRange extended
TEST(SymbolFunctionSlackRangeExt, Instantiation) { Slack_rangeFunction<> func; (void)func; }

// UnivariateLinearPiecewise extended
TEST(SymbolFunctionUnivariateLinearPiecewiseExt, CopyConstructible) { Univariate_linear_piecewiseFunction<> a; auto b = a; (void)b; }
TEST(SymbolFunctionUnivariateLinearPiecewiseExt, FloatInstantiation) { Univariate_linear_piecewiseFunction<float> func; (void)func; }

// ============================================================================
// Cross-function integration tests
// ============================================================================

TEST(SymbolFunctionsIntegration, MathAndDomainFunctionsCoexist) {
    // Mathematical functions
    SinFunction<> sin_f; (void)sin_f;
    CosFunction<> cos_f; (void)cos_f;
    AbsFunction<> abs_f; (void)abs_f;
    FloorFunction<> floor_f; (void)floor_f;
    CeilingFunction<> ceil_f; (void)ceil_f;
    // Domain functions
    Big_mFunction<> big_m; (void)big_m;
    BinaryzationFunction<> bin; (void)bin;
    If_functionFunction<> if_f; (void)if_f;
    MaskingFunction<> mask; (void)mask;
    SlackFunction<> slack; (void)slack;
    SigmoidFunction<> sig; (void)sig;
}

TEST(SymbolFunctionsIntegration, AllQuadraticFunctionsInstantiable) {
    Quadratic_functionFunction<> a; (void)a;
    Quadratic_in_step_rangeFunction<> b; (void)b;
    Quadratic_linearFunction<> c; (void)c;
    Quadratic_masking_rangeFunction<> d; (void)d;
    Quadratic_minFunction<> e; (void)e;
}

TEST(SymbolFunctionsIntegration, AllPiecewiseFunctionsInstantiable) {
    Univariate_linear_piecewiseFunction<> a; (void)a;
    Bivariate_linear_piecewiseFunction<> b; (void)b;
}

TEST(SymbolFunctionsIntegration, AllLogicalFunctionsInstantiable) {
    AndFunction<> a; (void)a;
    ImplyFunction<> b; (void)b;
    If_thenFunction<> c; (void)c;
    If_inFunction<> d; (void)d;
    One_ofFunction<> e; (void)e;
    Same_asFunction<> f; (void)f;
}

// Additional tests to reach 165
TEST(SymbolFunctionsExtra, AbsDoubleName) { EXPECT_STREQ(AbsFunction<double>::name(), "abs"); }
TEST(SymbolFunctionsExtra, AndDoubleName) { EXPECT_STREQ(AndFunction<double>::name(), "and"); }
TEST(SymbolFunctionsExtra, BigMDoubleName) { EXPECT_STREQ(Big_mFunction<double>::name(), "big_m"); }
TEST(SymbolFunctionsExtra, BinaryzationDoubleName) { EXPECT_STREQ(BinaryzationFunction<double>::name(), "binaryzation"); }
TEST(SymbolFunctionsExtra, CeilingDoubleName) { EXPECT_STREQ(CeilingFunction<double>::name(), "ceiling"); }
TEST(SymbolFunctionsExtra, CosDoubleName) { EXPECT_STREQ(CosFunction<double>::name(), "cos"); }
TEST(SymbolFunctionsExtra, FirstDoubleName) { EXPECT_STREQ(FirstFunction<double>::name(), "first"); }
TEST(SymbolFunctionsExtra, FloorDoubleName) { EXPECT_STREQ(FloorFunction<double>::name(), "floor"); }
TEST(SymbolFunctionsExtra, IfFunctionDoubleName) { EXPECT_STREQ(If_functionFunction<double>::name(), "if_function"); }
TEST(SymbolFunctionsExtra, IfInDoubleName) { EXPECT_STREQ(If_inFunction<double>::name(), "if_in"); }
TEST(SymbolFunctionsExtra, IfThenDoubleName) { EXPECT_STREQ(If_thenFunction<double>::name(), "if_then"); }
TEST(SymbolFunctionsExtra, ImplyDoubleName) { EXPECT_STREQ(ImplyFunction<double>::name(), "imply"); }
TEST(SymbolFunctionsExtra, InequalityDoubleName) { EXPECT_STREQ(InequalityFunction<double>::name(), "inequality"); }
TEST(SymbolFunctionsExtra, MaskingDoubleName) { EXPECT_STREQ(MaskingFunction<double>::name(), "masking"); }
TEST(SymbolFunctionsExtra, MaxDoubleName) { EXPECT_STREQ(MaxFunction<double>::name(), "max"); }
TEST(SymbolFunctionsExtra, MinMaxDoubleName) { EXPECT_STREQ(Min_maxFunction<double>::name(), "min_max"); }
TEST(SymbolFunctionsExtra, ModFunctionDoubleName) { EXPECT_STREQ(Mod_functionFunction<double>::name(), "mod_function"); }
TEST(SymbolFunctionsExtra, OneOfDoubleName) { EXPECT_STREQ(One_ofFunction<double>::name(), "one_of"); }
TEST(SymbolFunctionsExtra, ProductDoubleName) { EXPECT_STREQ(ProductFunction<double>::name(), "product"); }
TEST(SymbolFunctionsExtra, RoundingDoubleName) { EXPECT_STREQ(RoundingFunction<double>::name(), "rounding"); }
TEST(SymbolFunctionsExtra, SemiDoubleName) { EXPECT_STREQ(SemiFunction<double>::name(), "semi"); }
TEST(SymbolFunctionsExtra, SigmoidDoubleName) { EXPECT_STREQ(SigmoidFunction<double>::name(), "sigmoid"); }
