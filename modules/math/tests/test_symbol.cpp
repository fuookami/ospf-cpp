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

    // 创建多项式 p(x) = x^2 - 4
    auto p = Polynomial::quadratic(1.0, 0.0, -4.0, x);

    // p(2) = 0
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 0.0);
    // p(-2) = 0
    EXPECT_DOUBLE_EQ(p.evaluate(-2.0), 0.0);
    // p(0) = -4
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), -4.0);
}
