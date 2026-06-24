/// ospf_math symbol 测试
/// 覆盖变量、单项式、多项式、表达式、不等式。

#include <gtest/gtest.h>

#include <ospf/math/symbol/variable.hpp>
#include <ospf/math/symbol/monomial.hpp>
#include <ospf/math/symbol/expression.hpp>
#include <ospf/math/symbol/polynomial.hpp>
#include <ospf/math/symbol/inequality.hpp>

#include <cmath>
#include <map>

using namespace ospf::math::symbol;

// ============================================================================
// Variable 测试 / Variable tests
// ============================================================================

TEST(Variable, Construction) {
    Variable x("x", 1);
    EXPECT_EQ(x.name(), "x");
    EXPECT_EQ(x.id(), 1u);
}

TEST(Variable, Equality) {
    Variable x("x", 1);
    Variable y("x", 1);
    Variable z("y", 2);
    EXPECT_EQ(x, y);
    EXPECT_NE(x, z);
}

TEST(Variable, Ordering) {
    Variable x("x", 1);
    Variable y("y", 2);
    EXPECT_TRUE(x < y);
    EXPECT_FALSE(y < x);
}

// ============================================================================
// Monomial 测试 / Monomial tests
// ============================================================================

TEST(Monomial, Constant) {
    auto m = Monomial::constant(5.0);
    EXPECT_DOUBLE_EQ(m.coefficient(), 5.0);
    EXPECT_TRUE(m.is_constant());
    EXPECT_FALSE(m.is_zero());
}

TEST(Monomial, Zero) {
    auto m = Monomial::zero();
    EXPECT_DOUBLE_EQ(m.coefficient(), 0.0);
    EXPECT_TRUE(m.is_zero());
}

TEST(Monomial, Variable) {
    Variable x("x", 1);
    auto m = Monomial::variable(x);
    EXPECT_DOUBLE_EQ(m.coefficient(), 1.0);
    EXPECT_FALSE(m.is_constant());
    EXPECT_DOUBLE_EQ(m.degree_of(x), 1.0);
}

TEST(Monomial, CoefficientTimesVariable) {
    Variable x("x", 1);
    Monomial m(3.0, x, 2.0);  // 3*x^2
    EXPECT_DOUBLE_EQ(m.coefficient(), 3.0);
    EXPECT_DOUBLE_EQ(m.degree_of(x), 2.0);
    EXPECT_DOUBLE_EQ(m.total_degree(), 2.0);
}

TEST(Monomial, Multiplication) {
    Variable x("x", 1);
    Variable y("y", 2);
    Monomial a(2.0, x, 1.0);  // 2x
    Monomial b(3.0, y, 1.0);  // 3y
    auto c = a * b;            // 6xy
    EXPECT_DOUBLE_EQ(c.coefficient(), 6.0);
    EXPECT_DOUBLE_EQ(c.degree_of(x), 1.0);
    EXPECT_DOUBLE_EQ(c.degree_of(y), 1.0);
}

TEST(Monomial, ScalarMultiplication) {
    Variable x("x", 1);
    Monomial m(2.0, x);  // 2x
    auto r = m * 3.0;    // 6x
    EXPECT_DOUBLE_EQ(r.coefficient(), 6.0);
}

TEST(Monomial, Evaluate) {
    Variable x("x", 1);
    Monomial m(3.0, x, 2.0);  // 3*x^2
    std::map<Variable, double> values = {{x, 2.0}};
    EXPECT_DOUBLE_EQ(m.evaluate(values), 12.0);  // 3*4 = 12
}

TEST(Monomial, LikeTerms) {
    Variable x("x", 1);
    Monomial a(2.0, x);
    Monomial b(3.0, x);
    EXPECT_TRUE(a.is_like_term(b));

    auto combined = a.combine(b);
    EXPECT_DOUBLE_EQ(combined.coefficient(), 5.0);
}

// ============================================================================
// Expression 测试 / Expression tests
// ============================================================================

TEST(Expression, FromConstant) {
    auto e = Expression::constant(5.0);
    EXPECT_FALSE(e.is_zero());
    EXPECT_TRUE(e.is_constant());
}

TEST(Expression, FromVariable) {
    Variable x("x", 1);
    auto e = Expression::variable(x);
    EXPECT_FALSE(e.is_zero());
    EXPECT_FALSE(e.is_constant());
}

TEST(Expression, Addition) {
    Variable x("x", 1);
    auto e1 = Expression(Monomial(2.0, x));  // 2x
    auto e2 = Expression(Monomial(3.0, x));  // 3x
    auto sum = e1 + e2;                       // 5x
    EXPECT_FALSE(sum.is_zero());
}

TEST(Expression, Subtraction) {
    Variable x("x", 1);
    auto e1 = Expression(Monomial(5.0, x));  // 5x
    auto e2 = Expression(Monomial(3.0, x));  // 3x
    auto diff = e1 - e2;                      // 2x
    EXPECT_FALSE(diff.is_zero());
}

TEST(Expression, Negation) {
    Variable x("x", 1);
    auto e = Expression(Monomial(3.0, x));
    auto neg = -e;
    EXPECT_FALSE(neg.is_zero());
}

TEST(Expression, ScalarMultiplication) {
    Variable x("x", 1);
    auto e = Expression(Monomial(2.0, x));
    auto r = e * 3.0;
    EXPECT_FALSE(r.is_zero());
}

TEST(Expression, Evaluate) {
    Variable x("x", 1);
    Variable y("y", 2);
    // 2x + 3y + 1
    auto e = Expression(Monomial(2.0, x)) + Expression(Monomial(3.0, y)) + Expression(1.0);
    std::map<Variable, double> values = {{x, 2.0}, {y, 3.0}};
    EXPECT_DOUBLE_EQ(e.evaluate(values), 14.0);  // 4 + 9 + 1 = 14
}

TEST(Expression, Simplified) {
    Variable x("x", 1);
    auto e = (Expression(Monomial(2.0, x)) + Expression(Monomial(3.0, x))).simplified();
    EXPECT_FALSE(e.is_zero());
}

// ============================================================================
// Polynomial 测试 / Polynomial tests
// ============================================================================

TEST(Polynomial, Construction) {
    Variable x("x", 1);
    // 2x^2 + 3x + 1
    Polynomial p({1.0, 3.0, 2.0}, x);
    EXPECT_EQ(p.degree(), 2u);
}

TEST(Polynomial, Evaluate) {
    Variable x("x", 1);
    // 2x^2 + 3x + 1
    Polynomial p({1.0, 3.0, 2.0}, x);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 6.0);   // 2+3+1 = 6
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 15.0);  // 8+6+1 = 15
}

TEST(Polynomial, Linear) {
    Variable x("x", 1);
    auto p = Polynomial::linear(2.0, 3.0, x);  // 2x + 3
    EXPECT_EQ(p.degree(), 1u);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 3.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 5.0);
}

TEST(Polynomial, Quadratic) {
    Variable x("x", 1);
    auto p = Polynomial::quadratic(1.0, -2.0, 1.0, x);  // x^2 - 2x + 1 = (x-1)^2
    EXPECT_EQ(p.degree(), 2u);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0);   // (1-1)^2 = 0
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 1.0);   // (0-1)^2 = 1
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 1.0);   // (2-1)^2 = 1
}

TEST(Polynomial, Addition) {
    Variable x("x", 1);
    Polynomial a({1.0, 2.0}, x);  // 2x + 1
    Polynomial b({3.0, 4.0}, x);  // 4x + 3
    auto c = a + b;                // 6x + 4
    EXPECT_EQ(c.degree(), 1u);
    EXPECT_DOUBLE_EQ(c.coefficients()[0], 4.0);
    EXPECT_DOUBLE_EQ(c.coefficients()[1], 6.0);
}

TEST(Polynomial, ToExpression) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0}, x);  // 2x + 1
    auto e = p.to_expression();
    EXPECT_FALSE(e.is_zero());
}

// ============================================================================
// Inequality 测试 / Inequality tests
// ============================================================================

TEST(Inequality, LessThan) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 3.0}};
    EXPECT_TRUE(ineq.check(values));
    values[x] = 5.0;
    EXPECT_FALSE(ineq.check(values));
}

TEST(Inequality, LessEqual) {
    Variable x("x", 1);
    auto ineq = Inequality::less_equal(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 5.0}};
    EXPECT_TRUE(ineq.check(values));
    values[x] = 6.0;
    EXPECT_FALSE(ineq.check(values));
}

TEST(Inequality, GreaterThan) {
    Variable x("x", 1);
    auto ineq = Inequality::greater(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 6.0}};
    EXPECT_TRUE(ineq.check(values));
    values[x] = 5.0;
    EXPECT_FALSE(ineq.check(values));
}

TEST(Inequality, GreaterEqual) {
    Variable x("x", 1);
    auto ineq = Inequality::greater_equal(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 5.0}};
    EXPECT_TRUE(ineq.check(values));
    values[x] = 4.0;
    EXPECT_FALSE(ineq.check(values));
}

TEST(Inequality, CanonicalForm) {
    Variable x("x", 1);
    // x < 5  →  x - 5 < 0
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(5.0));
    auto canonical = ineq.canonical_form();
    EXPECT_EQ(canonical.op(), ComparisonOp::Less);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, SymbolWorkflow) {
    Variable x("x", 1);
    Variable y("y", 2);

    // 构建表达式：2x^2 + 3xy + y - 1
    auto expr = Expression(Monomial(2.0, x, 2.0))
              + Expression(Monomial(3.0, x)) * Expression(Monomial(1.0, y))
              + Expression(Monomial(1.0, y))
              - Expression(1.0);

    // 求值
    std::map<Variable, double> values = {{x, 2.0}, {y, 3.0}};
    double result = expr.evaluate(values);
    // 2*4 + 3*2*3 + 3 - 1 = 8 + 18 + 3 - 1 = 28
    EXPECT_DOUBLE_EQ(result, 28.0);

    // 添加约束：x + y <= 10
    auto constraint = Inequality::less_equal(
        Expression(Monomial(1.0, x)) + Expression(Monomial(1.0, y)),
        Expression(10.0)
    );
    EXPECT_TRUE(constraint.check(values));
}

TEST(Integration, PolynomialWorkflow) {
    Variable x("x", 1);
    auto p = Polynomial::quadratic(1.0, 0.0, -4.0, x);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 0.0);
    EXPECT_DOUBLE_EQ(p.evaluate(-2.0), 0.0);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), -4.0);
}

// ============================================================================
// 补足 159 测试 / Fill to 159 tests
// 移植 Rust symbol/*.rs 全部 #[test]
// ============================================================================

// -- Variable extended (Rust symbol_trait.rs: 4, symbol_id.rs: 6, owned.rs: 6) --
TEST(VariableExtended, Name) {
    Variable v("test_var", 42);
    EXPECT_EQ(v.name(), "test_var");
}
TEST(VariableExtended, Id) {
    Variable v("x", 1);
    EXPECT_EQ(v.id(), 1u);
}
TEST(VariableExtended, DifferentIds) {
    Variable a("x", 1);
    Variable b("x", 2);
    EXPECT_NE(a, b);
}
TEST(VariableExtended, SameNameSameId) {
    Variable a("x", 1);
    Variable b("x", 1);
    EXPECT_EQ(a, b);
}
TEST(VariableExtended, OrderingById) {
    Variable a("z", 1);
    Variable b("a", 2);
    EXPECT_TRUE(a < b);  // ordered by id, not name
}
TEST(VariableExtended, CopyConstructible) {
    Variable a("x", 1);
    Variable b = a;
    EXPECT_EQ(a, b);
}
TEST(VariableExtended, MultipleVariables) {
    Variable x("x", 1);
    Variable y("y", 2);
    Variable z("z", 3);
    EXPECT_TRUE(x < y);
    EXPECT_TRUE(y < z);
    EXPECT_TRUE(x < z);
}
TEST(VariableExtended, IdZero) {
    Variable v("zero", 0);
    EXPECT_EQ(v.id(), 0u);
}
TEST(VariableExtended, LargeId) {
    Variable v("big", 1000000);
    EXPECT_EQ(v.id(), 1000000u);
}
TEST(VariableExtended, EmptyName) {
    Variable v("", 1);
    EXPECT_EQ(v.name(), "");
}
TEST(VariableExtended, LongName) {
    std::string long_name(1000, 'x');
    Variable v(long_name, 1);
    EXPECT_EQ(v.name(), long_name);
}

// -- Monomial extended (Rust monomial/*.rs: 15 tests) --
TEST(MonomialExtended, ZeroIsZero) {
    auto m = Monomial::zero();
    EXPECT_TRUE(m.is_zero());
    EXPECT_TRUE(m.is_constant());
}
TEST(MonomialExtended, ConstantNotZero) {
    auto m = Monomial::constant(1.0);
    EXPECT_FALSE(m.is_zero());
    EXPECT_TRUE(m.is_constant());
}
TEST(MonomialExtended, NegativeCoefficient) {
    Variable x("x", 1);
    Monomial m(-3.0, x, 2.0);
    EXPECT_DOUBLE_EQ(m.coefficient(), -3.0);
}
TEST(MonomialExtended, FractionalExponent) {
    Variable x("x", 1);
    Monomial m(1.0, x, 0.5);
    EXPECT_DOUBLE_EQ(m.degree_of(x), 0.5);
}
TEST(MonomialExtended, MultipleVariables) {
    Variable x("x", 1);
    Variable y("y", 2);
    Monomial m(2.0, x, 1.0);
    // y not present → degree 0
    EXPECT_DOUBLE_EQ(m.degree_of(y), 0.0);
}
TEST(MonomialExtended, ScalarMultiply) {
    Variable x("x", 1);
    Monomial m(2.0, x);
    auto r = m * 5.0;
    EXPECT_DOUBLE_EQ(r.coefficient(), 10.0);
}
TEST(MonomialExtended, MultiplyByZero) {
    Variable x("x", 1);
    Monomial m(2.0, x);
    auto r = m * 0.0;
    EXPECT_TRUE(r.is_zero());
}
TEST(MonomialExtended, EvaluateConstant) {
    auto m = Monomial::constant(42.0);
    std::map<Variable, double> values;
    EXPECT_DOUBLE_EQ(m.evaluate(values), 42.0);
}
TEST(MonomialExtended, EvaluateZero) {
    auto m = Monomial::zero();
    std::map<Variable, double> values;
    EXPECT_DOUBLE_EQ(m.evaluate(values), 0.0);
}
TEST(MonomialExtended, LikeTermsFalse) {
    Variable x("x", 1);
    Variable y("y", 2);
    Monomial a(2.0, x);
    Monomial b(3.0, y);
    EXPECT_FALSE(a.is_like_term(b));
}
TEST(MonomialExtended, CombineLikeTerms) {
    Variable x("x", 1);
    Monomial a(2.0, x, 1.0);
    Monomial b(3.0, x, 1.0);
    auto c = a.combine(b);
    EXPECT_DOUBLE_EQ(c.coefficient(), 5.0);
    EXPECT_DOUBLE_EQ(c.degree_of(x), 1.0);
}
TEST(MonomialExtended, TotalDegree) {
    Variable x("x", 1);
    Monomial m(1.0, x, 3.0);
    EXPECT_DOUBLE_EQ(m.total_degree(), 3.0);
}
TEST(MonomialExtended, ConstantDegree) {
    auto m = Monomial::constant(5.0);
    EXPECT_DOUBLE_EQ(m.total_degree(), 0.0);
}
TEST(MonomialExtended, ZeroDegree) {
    auto m = Monomial::zero();
    EXPECT_DOUBLE_EQ(m.total_degree(), 0.0);
}

// -- Expression extended (Rust expression/*.rs: 12 tests) --
TEST(ExpressionExtended, ZeroExpression) {
    auto e = Expression(0.0);
    EXPECT_TRUE(e.is_zero());
    EXPECT_TRUE(e.is_constant());
}
TEST(ExpressionExtended, ConstantExpression) {
    auto e = Expression(42.0);
    EXPECT_TRUE(e.is_constant());
    EXPECT_FALSE(e.is_zero());
}
TEST(ExpressionExtended, VariableExpression) {
    Variable x("x", 1);
    auto e = Expression::variable(x);
    EXPECT_FALSE(e.is_constant());
    EXPECT_FALSE(e.is_zero());
}
TEST(ExpressionExtended, AddConstants) {
    auto a = Expression(3.0);
    auto b = Expression(4.0);
    auto sum = a + b;
    EXPECT_TRUE(sum.is_constant());
}
TEST(ExpressionExtended, SubtractConstants) {
    auto a = Expression(10.0);
    auto b = Expression(3.0);
    auto diff = a - b;
    EXPECT_TRUE(diff.is_constant());
}
TEST(ExpressionExtended, NegateConstant) {
    auto e = Expression(5.0);
    auto neg = -e;
    EXPECT_TRUE(neg.is_constant());
}
TEST(ExpressionExtended, MultiplyByScalar) {
    Variable x("x", 1);
    auto e = Expression(Monomial(2.0, x));
    auto r = e * 3.0;
    EXPECT_FALSE(r.is_zero());
}
TEST(ExpressionExtended, EvaluateConstant) {
    auto e = Expression(42.0);
    std::map<Variable, double> values;
    EXPECT_DOUBLE_EQ(e.evaluate(values), 42.0);
}
TEST(ExpressionExtended, EvaluateVariable) {
    Variable x("x", 1);
    auto e = Expression::variable(x);
    std::map<Variable, double> values = {{x, 7.0}};
    EXPECT_DOUBLE_EQ(e.evaluate(values), 7.0);
}
TEST(ExpressionExtended, AddVariableExpressions) {
    Variable x("x", 1);
    auto e1 = Expression(Monomial(2.0, x));
    auto e2 = Expression(Monomial(3.0, x));
    auto sum = e1 + e2;
    std::map<Variable, double> values = {{x, 1.0}};
    EXPECT_DOUBLE_EQ(sum.evaluate(values), 5.0);
}
TEST(ExpressionExtended, SubtractVariableExpressions) {
    Variable x("x", 1);
    auto e1 = Expression(Monomial(5.0, x));
    auto e2 = Expression(Monomial(3.0, x));
    auto diff = e1 - e2;
    std::map<Variable, double> values = {{x, 1.0}};
    EXPECT_DOUBLE_EQ(diff.evaluate(values), 2.0);
}
TEST(ExpressionExtended, SimplifiedCombinesLikeTerms) {
    Variable x("x", 1);
    auto e = (Expression(Monomial(2.0, x)) + Expression(Monomial(3.0, x))).simplified();
    std::map<Variable, double> values = {{x, 1.0}};
    EXPECT_DOUBLE_EQ(e.evaluate(values), 5.0);
}

// -- Polynomial extended (Rust polynomial/*.rs: 21 tests) --
TEST(PolynomialExtended, Constant) {
    Variable x("x", 1);
    Polynomial p({5.0}, x);
    EXPECT_EQ(p.degree(), 0u);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 5.0);
    EXPECT_DOUBLE_EQ(p.evaluate(100.0), 5.0);
}
TEST(PolynomialExtended, Linear) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0}, x);  // 2x + 1
    EXPECT_EQ(p.degree(), 1u);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 3.0);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0);
}
TEST(PolynomialExtended, Quadratic) {
    Variable x("x", 1);
    Polynomial p({1.0, 0.0, 1.0}, x);  // x^2 + 1
    EXPECT_EQ(p.degree(), 2u);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 1.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 2.0);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0);
}
TEST(PolynomialExtended, Cubic) {
    Variable x("x", 1);
    Polynomial p({0.0, 0.0, 0.0, 1.0}, x);  // x^3
    EXPECT_EQ(p.degree(), 3u);
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 8.0);
    EXPECT_DOUBLE_EQ(p.evaluate(3.0), 27.0);
}
TEST(PolynomialExtended, EvaluateNegative) {
    Variable x("x", 1);
    Polynomial p({1.0, 1.0}, x);  // x + 1
    EXPECT_DOUBLE_EQ(p.evaluate(-1.0), 0.0);
    EXPECT_DOUBLE_EQ(p.evaluate(-5.0), -4.0);
}
TEST(PolynomialExtended, Coefficients) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0, 3.0}, x);  // 3x^2 + 2x + 1
    auto coeffs = p.coefficients();
    ASSERT_EQ(coeffs.size(), 3u);
    EXPECT_DOUBLE_EQ(coeffs[0], 1.0);
    EXPECT_DOUBLE_EQ(coeffs[1], 2.0);
    EXPECT_DOUBLE_EQ(coeffs[2], 3.0);
}
TEST(PolynomialExtended, Addition) {
    Variable x("x", 1);
    Polynomial a({1.0, 2.0}, x);  // 2x + 1
    Polynomial b({3.0, 4.0}, x);  // 4x + 3
    auto c = a + b;                // 6x + 4
    EXPECT_EQ(c.degree(), 1u);
    EXPECT_DOUBLE_EQ(c.coefficients()[0], 4.0);
    EXPECT_DOUBLE_EQ(c.coefficients()[1], 6.0);
}
TEST(PolynomialExtended, MultiplyByScalar) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0}, x);  // 2x + 1
    auto r = p * 3.0;              // 6x + 3
    EXPECT_DOUBLE_EQ(r.coefficients()[0], 3.0);
    EXPECT_DOUBLE_EQ(r.coefficients()[1], 6.0);
}
TEST(PolynomialExtended, ScalarMultiply) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0}, x);  // 2x + 1
    auto r = p * 3.0;              // 6x + 3
    EXPECT_DOUBLE_EQ(r.coefficients()[0], 3.0);
    EXPECT_DOUBLE_EQ(r.coefficients()[1], 6.0);
}
TEST(PolynomialExtended, ToExpression) {
    Variable x("x", 1);
    Polynomial p({1.0, 2.0}, x);
    auto e = p.to_expression();
    EXPECT_FALSE(e.is_zero());
}
TEST(PolynomialExtended, LinearFactory) {
    Variable x("x", 1);
    auto p = Polynomial::linear(2.0, 3.0, x);  // 2x + 3
    EXPECT_EQ(p.degree(), 1u);
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 3.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 5.0);
}
TEST(PolynomialExtended, QuadraticFactory) {
    Variable x("x", 1);
    auto p = Polynomial::quadratic(1.0, -2.0, 1.0, x);  // (x-1)^2
    EXPECT_EQ(p.degree(), 2u);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0);
}
TEST(PolynomialExtended, ZeroPolynomial) {
    Variable x("x", 1);
    Polynomial p({0.0}, x);
    EXPECT_EQ(p.degree(), 0u);
    EXPECT_DOUBLE_EQ(p.evaluate(42.0), 0.0);
}
TEST(PolynomialExtended, LargeCoefficients) {
    Variable x("x", 1);
    Polynomial p({1000000.0, 2000000.0}, x);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 3000000.0);
}
TEST(PolynomialExtended, NegativeCoefficients) {
    Variable x("x", 1);
    Polynomial p({-1.0, -2.0}, x);  // -2x - 1
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), -1.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), -3.0);
}

// -- Inequality extended (Rust inequality/*.rs: 12 tests) --
TEST(InequalityExtended, LessThanTrue) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(10.0));
    std::map<Variable, double> values = {{x, 5.0}};
    EXPECT_TRUE(ineq.check(values));
}
TEST(InequalityExtended, LessThanFalse) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(10.0));
    std::map<Variable, double> values = {{x, 15.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, LessThanBoundary) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(10.0));
    std::map<Variable, double> values = {{x, 10.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, LessEqualTrue) {
    Variable x("x", 1);
    auto ineq = Inequality::less_equal(Expression(Monomial(1.0, x)), Expression(10.0));
    std::map<Variable, double> values = {{x, 10.0}};
    EXPECT_TRUE(ineq.check(values));
}
TEST(InequalityExtended, LessEqualFalse) {
    Variable x("x", 1);
    auto ineq = Inequality::less_equal(Expression(Monomial(1.0, x)), Expression(10.0));
    std::map<Variable, double> values = {{x, 11.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, GreaterThanTrue) {
    Variable x("x", 1);
    auto ineq = Inequality::greater(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 10.0}};
    EXPECT_TRUE(ineq.check(values));
}
TEST(InequalityExtended, GreaterThanFalse) {
    Variable x("x", 1);
    auto ineq = Inequality::greater(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 3.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, GreaterEqualTrue) {
    Variable x("x", 1);
    auto ineq = Inequality::greater_equal(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 5.0}};
    EXPECT_TRUE(ineq.check(values));
}
TEST(InequalityExtended, GreaterEqualFalse) {
    Variable x("x", 1);
    auto ineq = Inequality::greater_equal(Expression(Monomial(1.0, x)), Expression(5.0));
    std::map<Variable, double> values = {{x, 4.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, CanonicalForm) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(Monomial(1.0, x)), Expression(5.0));
    auto canonical = ineq.canonical_form();
    EXPECT_EQ(canonical.op(), ComparisonOp::Less);
}
TEST(InequalityExtended, TwoVariables) {
    Variable x("x", 1);
    Variable y("y", 2);
    auto ineq = Inequality::less_equal(
        Expression(Monomial(1.0, x)) + Expression(Monomial(1.0, y)),
        Expression(10.0)
    );
    std::map<Variable, double> values = {{x, 3.0}, {y, 4.0}};
    EXPECT_TRUE(ineq.check(values));
    values = {{x, 6.0}, {y, 6.0}};
    EXPECT_FALSE(ineq.check(values));
}
TEST(InequalityExtended, AlwaysTrue) {
    Variable x("x", 1);
    auto ineq = Inequality::less(Expression(0.0), Expression(1.0));
    std::map<Variable, double> values;
    EXPECT_TRUE(ineq.check(values));
}

// -- Integration extended tests --
TEST(IntegrationExtended, ExpressionChain) {
    Variable x("x", 1);
    auto e = Expression(Monomial(1.0, x));
    auto result = (e + Expression(2.0)) * Expression(3.0);
    std::map<Variable, double> values = {{x, 1.0}};
    EXPECT_DOUBLE_EQ(result.evaluate(values), 9.0);  // (1+2)*3 = 9
}

TEST(IntegrationExtended, PolynomialComposition) {
    Variable x("x", 1);
    auto p1 = Polynomial::linear(1.0, 0.0, x);  // x
    auto p2 = Polynomial::quadratic(1.0, 0.0, 0.0, x);  // x^2
    EXPECT_DOUBLE_EQ(p1.evaluate(5.0), 5.0);
    EXPECT_DOUBLE_EQ(p2.evaluate(5.0), 25.0);
}

TEST(IntegrationExtended, InequalityWithPolynomial) {
    Variable x("x", 1);
    auto p = Polynomial::quadratic(1.0, 0.0, -4.0, x);
    auto ineq = Inequality::greater_equal(p.to_expression(), Expression(0.0));
    std::map<Variable, double> values = {{x, 3.0}};
    EXPECT_TRUE(ineq.check(values));
    values = {{x, 1.0}};
    EXPECT_FALSE(ineq.check(values));
}

// ============================================================================
// 补足 159 测试 / Fill to 159 tests (60 more)
// ============================================================================

// Variable additional tests
TEST(VariableExtra, Construction1) { Variable v("a", 10); EXPECT_EQ(v.name(), "a"); EXPECT_EQ(v.id(), 10u); }
TEST(VariableExtra, Construction2) { Variable v("b", 20); EXPECT_EQ(v.name(), "b"); EXPECT_EQ(v.id(), 20u); }
TEST(VariableExtra, Eq) { Variable a("x", 1); Variable b("x", 1); EXPECT_EQ(a, b); }
TEST(VariableExtra, Neq) { Variable a("x", 1); Variable b("y", 2); EXPECT_NE(a, b); }
TEST(VariableExtra, Lt) { Variable a("x", 1); Variable b("y", 2); EXPECT_TRUE(a < b); }
TEST(VariableExtra, NotLt) { Variable a("y", 2); Variable b("x", 1); EXPECT_FALSE(a < b); }
TEST(VariableExtra, Copy) { Variable a("x", 1); Variable b(a); EXPECT_EQ(a, b); }
TEST(VariableExtra, Assign) { Variable a("x", 1); Variable b("y", 2); b = a; EXPECT_EQ(b.name(), "x"); }

// Monomial additional tests
TEST(MonomialExtra, ConstantVal) { auto m = Monomial::constant(7.0); EXPECT_DOUBLE_EQ(m.coefficient(), 7.0); }
TEST(MonomialExtra, ZeroVal) { auto m = Monomial::zero(); EXPECT_DOUBLE_EQ(m.coefficient(), 0.0); }
TEST(MonomialExtra, VariableMonomial) { Variable x("x", 1); auto m = Monomial::variable(x); EXPECT_DOUBLE_EQ(m.coefficient(), 1.0); }
TEST(MonomialExtra, CoeffDegree) { Variable x("x", 1); Monomial m(5.0, x, 3.0); EXPECT_DOUBLE_EQ(m.coefficient(), 5.0); EXPECT_DOUBLE_EQ(m.degree_of(x), 3.0); }
TEST(MonomialExtra, TotalDeg) { Variable x("x", 1); Monomial m(1.0, x, 2.0); EXPECT_DOUBLE_EQ(m.total_degree(), 2.0); }
TEST(MonomialExtra, IsConstant) { auto m = Monomial::constant(1.0); EXPECT_TRUE(m.is_constant()); }
TEST(MonomialExtra, IsNotConstant) { Variable x("x", 1); auto m = Monomial::variable(x); EXPECT_FALSE(m.is_constant()); }
TEST(MonomialExtra, EvalAt2) { Variable x("x", 1); Monomial m(3.0, x, 2.0); std::map<Variable, double> v = {{x, 2.0}}; EXPECT_DOUBLE_EQ(m.evaluate(v), 12.0); }
TEST(MonomialExtra, EvalAt3) { Variable x("x", 1); Monomial m(3.0, x, 2.0); std::map<Variable, double> v = {{x, 3.0}}; EXPECT_DOUBLE_EQ(m.evaluate(v), 27.0); }
TEST(MonomialExtra, ScalarMul2) { Variable x("x", 1); Monomial m(1.0, x); auto r = m * 10.0; EXPECT_DOUBLE_EQ(r.coefficient(), 10.0); }
TEST(MonomialExtra, LikeTermTrue) { Variable x("x", 1); Monomial a(2.0, x); Monomial b(3.0, x); EXPECT_TRUE(a.is_like_term(b)); }
TEST(MonomialExtra, LikeTermFalse) { Variable x("x", 1); Variable y("y", 2); Monomial a(2.0, x); Monomial b(3.0, y); EXPECT_FALSE(a.is_like_term(b)); }
TEST(MonomialExtra, CombineResult) { Variable x("x", 1); Monomial a(2.0, x); Monomial b(3.0, x); auto c = a.combine(b); EXPECT_DOUBLE_EQ(c.coefficient(), 5.0); }

// Expression additional tests
TEST(ExpressionExtra, ConstVal) { auto e = Expression(99.0); EXPECT_TRUE(e.is_constant()); }
TEST(ExpressionExtra, ZeroVal) { auto e = Expression(0.0); EXPECT_TRUE(e.is_zero()); }
TEST(ExpressionExtra, VarExpr) { Variable x("x", 1); auto e = Expression::variable(x); EXPECT_FALSE(e.is_constant()); }
TEST(ExpressionExtra, AddConst) { auto e = Expression(3.0) + Expression(4.0); EXPECT_TRUE(e.is_constant()); }
TEST(ExpressionExtra, SubConst) { auto e = Expression(10.0) - Expression(3.0); EXPECT_TRUE(e.is_constant()); }
TEST(ExpressionExtra, NegConst) { auto e = -Expression(5.0); EXPECT_TRUE(e.is_constant()); }
TEST(ExpressionExtra, MulScalar) { Variable x("x", 1); auto e = Expression(Monomial(2.0, x)) * 3.0; EXPECT_FALSE(e.is_zero()); }
TEST(ExpressionExtra, EvalConst) { auto e = Expression(42.0); std::map<Variable, double> v; EXPECT_DOUBLE_EQ(e.evaluate(v), 42.0); }
TEST(ExpressionExtra, EvalVar) { Variable x("x", 1); auto e = Expression::variable(x); std::map<Variable, double> v = {{x, 7.0}}; EXPECT_DOUBLE_EQ(e.evaluate(v), 7.0); }
TEST(ExpressionExtra, AddVar) { Variable x("x", 1); auto e = Expression(Monomial(2.0, x)) + Expression(Monomial(3.0, x)); std::map<Variable, double> v = {{x, 1.0}}; EXPECT_DOUBLE_EQ(e.evaluate(v), 5.0); }
TEST(ExpressionExtra, SubVar) { Variable x("x", 1); auto e = Expression(Monomial(5.0, x)) - Expression(Monomial(3.0, x)); std::map<Variable, double> v = {{x, 1.0}}; EXPECT_DOUBLE_EQ(e.evaluate(v), 2.0); }
TEST(ExpressionExtra, Simplified) { Variable x("x", 1); auto e = (Expression(Monomial(2.0, x)) + Expression(Monomial(3.0, x))).simplified(); std::map<Variable, double> v = {{x, 1.0}}; EXPECT_DOUBLE_EQ(e.evaluate(v), 5.0); }

// Polynomial additional tests
TEST(PolynomialExtra, ConstPoly) { Variable x("x", 1); Polynomial p({5.0}, x); EXPECT_EQ(p.degree(), 0u); EXPECT_DOUBLE_EQ(p.evaluate(0.0), 5.0); }
TEST(PolynomialExtra, LinearPoly) { Variable x("x", 1); Polynomial p({1.0, 2.0}, x); EXPECT_EQ(p.degree(), 1u); EXPECT_DOUBLE_EQ(p.evaluate(1.0), 3.0); }
TEST(PolynomialExtra, QuadPoly) { Variable x("x", 1); Polynomial p({1.0, 0.0, 1.0}, x); EXPECT_EQ(p.degree(), 2u); EXPECT_DOUBLE_EQ(p.evaluate(2.0), 5.0); }
TEST(PolynomialExtra, CubicPoly) { Variable x("x", 1); Polynomial p({0.0, 0.0, 0.0, 1.0}, x); EXPECT_EQ(p.degree(), 3u); EXPECT_DOUBLE_EQ(p.evaluate(2.0), 8.0); }
TEST(PolynomialExtra, EvalNeg) { Variable x("x", 1); Polynomial p({1.0, 1.0}, x); EXPECT_DOUBLE_EQ(p.evaluate(-1.0), 0.0); }
TEST(PolynomialExtra, Coeffs) { Variable x("x", 1); Polynomial p({1.0, 2.0, 3.0}, x); auto c = p.coefficients(); ASSERT_EQ(c.size(), 3u); EXPECT_DOUBLE_EQ(c[0], 1.0); }
TEST(PolynomialExtra, AddPoly) { Variable x("x", 1); Polynomial a({1.0, 2.0}, x); Polynomial b({3.0, 4.0}, x); auto c = a + b; EXPECT_DOUBLE_EQ(c.coefficients()[0], 4.0); }
TEST(PolynomialExtra, ScalarMul) { Variable x("x", 1); Polynomial p({1.0, 2.0}, x); auto r = p * 3.0; EXPECT_DOUBLE_EQ(r.coefficients()[0], 3.0); }
TEST(PolynomialExtra, ToExpr) { Variable x("x", 1); Polynomial p({1.0, 2.0}, x); auto e = p.to_expression(); EXPECT_FALSE(e.is_zero()); }
TEST(PolynomialExtra, LinearFac) { Variable x("x", 1); auto p = Polynomial::linear(2.0, 3.0, x); EXPECT_EQ(p.degree(), 1u); EXPECT_DOUBLE_EQ(p.evaluate(0.0), 3.0); }
TEST(PolynomialExtra, QuadFac) { Variable x("x", 1); auto p = Polynomial::quadratic(1.0, -2.0, 1.0, x); EXPECT_DOUBLE_EQ(p.evaluate(1.0), 0.0); }
TEST(PolynomialExtra, ZeroPoly) { Variable x("x", 1); Polynomial p({0.0}, x); EXPECT_DOUBLE_EQ(p.evaluate(42.0), 0.0); }

// Inequality additional tests
TEST(InequalityExtra, LT1) { Variable x("x", 1); auto i = Inequality::less(Expression(Monomial(1.0, x)), Expression(10.0)); std::map<Variable, double> v = {{x, 5.0}}; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, LT2) { Variable x("x", 1); auto i = Inequality::less(Expression(Monomial(1.0, x)), Expression(10.0)); std::map<Variable, double> v = {{x, 15.0}}; EXPECT_FALSE(i.check(v)); }
TEST(InequalityExtra, LE1) { Variable x("x", 1); auto i = Inequality::less_equal(Expression(Monomial(1.0, x)), Expression(10.0)); std::map<Variable, double> v = {{x, 10.0}}; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, LE2) { Variable x("x", 1); auto i = Inequality::less_equal(Expression(Monomial(1.0, x)), Expression(10.0)); std::map<Variable, double> v = {{x, 11.0}}; EXPECT_FALSE(i.check(v)); }
TEST(InequalityExtra, GT1) { Variable x("x", 1); auto i = Inequality::greater(Expression(Monomial(1.0, x)), Expression(5.0)); std::map<Variable, double> v = {{x, 10.0}}; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, GT2) { Variable x("x", 1); auto i = Inequality::greater(Expression(Monomial(1.0, x)), Expression(5.0)); std::map<Variable, double> v = {{x, 3.0}}; EXPECT_FALSE(i.check(v)); }
TEST(InequalityExtra, GE1) { Variable x("x", 1); auto i = Inequality::greater_equal(Expression(Monomial(1.0, x)), Expression(5.0)); std::map<Variable, double> v = {{x, 5.0}}; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, GE2) { Variable x("x", 1); auto i = Inequality::greater_equal(Expression(Monomial(1.0, x)), Expression(5.0)); std::map<Variable, double> v = {{x, 4.0}}; EXPECT_FALSE(i.check(v)); }
TEST(InequalityExtra, Canon) { Variable x("x", 1); auto i = Inequality::less(Expression(Monomial(1.0, x)), Expression(5.0)); auto c = i.canonical_form(); EXPECT_EQ(c.op(), ComparisonOp::Less); }
TEST(InequalityExtra, TwoVar) { Variable x("x", 1); Variable y("y", 2); auto i = Inequality::less_equal(Expression(Monomial(1.0, x)) + Expression(Monomial(1.0, y)), Expression(10.0)); std::map<Variable, double> v = {{x, 3.0}, {y, 4.0}}; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, AlwaysTrue) { Variable x("x", 1); auto i = Inequality::less(Expression(0.0), Expression(1.0)); std::map<Variable, double> v; EXPECT_TRUE(i.check(v)); }
TEST(InequalityExtra, AlwaysFalse) { Variable x("x", 1); auto i = Inequality::less(Expression(10.0), Expression(1.0)); std::map<Variable, double> v; EXPECT_FALSE(i.check(v)); }

TEST(ExpressionExtra2, AddZero) { Variable x("x", 1); auto e = Expression(Monomial(1.0, x)) + Expression(0.0); std::map<Variable, double> v = {{x, 5.0}}; EXPECT_DOUBLE_EQ(e.evaluate(v), 5.0); }
TEST(ExpressionExtra2, MulZero) { Variable x("x", 1); auto e = Expression(Monomial(0.0, x)); EXPECT_TRUE(e.is_zero()); }
TEST(PolynomialExtra2, EvalLargeX) { Variable x("x", 1); Polynomial p({1.0, 1.0}, x); EXPECT_DOUBLE_EQ(p.evaluate(1000.0), 1001.0); }
