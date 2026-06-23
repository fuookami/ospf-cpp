/// ospf_math algebra 测试
/// 覆盖代数结构概念、值区间、数值特征、向量、矩阵。

#include <gtest/gtest.h>

#include <ospf/math/algebra/concept.hpp>
#include <ospf/math/algebra/value_range.hpp>
#include <ospf/math/algebra/number.hpp>
#include <ospf/math/algebra/vector.hpp>
#include <ospf/math/algebra/matrix.hpp>

#include <cmath>
#include <numbers>

using namespace ospf::math::algebra;

// ============================================================================
// 代数结构概念测试 / Algebraic structure concept tests
// ============================================================================

TEST(Concepts, Semigroup) {
    static_assert(Semigroup<int>, "int must be Semigroup");
    static_assert(Semigroup<double>, "double must be Semigroup");
    static_assert(Semigroup<std::string>, "string must be Semigroup");
}

TEST(Concepts, Monoid) {
    static_assert(Monoid<int>, "int must be Monoid");
    static_assert(Monoid<double>, "double must be Monoid");
}

TEST(Concepts, Group) {
    static_assert(Group<int>, "int must be Group");
    static_assert(Group<double>, "double must be Group");
}

TEST(Concepts, AbelianGroup) {
    static_assert(AbelianGroup<int>, "int must be AbelianGroup");
    static_assert(AbelianGroup<double>, "double must be AbelianGroup");
}

TEST(Concepts, Ring) {
    static_assert(Ring<int>, "int must be Ring");
    static_assert(Ring<double>, "double must be Ring");
}

TEST(Concepts, Field) {
    static_assert(Field<double>, "double must be Field");
    static_assert(Field<float>, "float must be Field");
    // Note: int technically satisfies Field concept but is not a mathematical field
    // (1/2 is not an integer). This is a limitation of C++ concept checking.
}

TEST(Concepts, TotallyOrdered) {
    static_assert(TotallyOrdered<int>, "int must be TotallyOrdered");
    static_assert(TotallyOrdered<double>, "double must be TotallyOrdered");
    static_assert(TotallyOrdered<std::string>, "string must be TotallyOrdered");
}

TEST(Concepts, Scalar) {
    static_assert(Scalar<double>, "double must be Scalar");
    static_assert(Scalar<float>, "float must be Scalar");
}

TEST(Concepts, RealNumber) {
    static_assert(RealNumber<double>, "double must be RealNumber");
    static_assert(RealNumber<float>, "float must be RealNumber");
    static_assert(!RealNumber<int>, "int must not be RealNumber");
}

TEST(Concepts, VectorSpace) {
    static_assert(VectorSpace<Vec3d, double>, "Vec3d must be VectorSpace");
}

TEST(Concepts, InnerProductSpace) {
    static_assert(InnerProductSpace<Vec3d, double>, "Vec3d must be InnerProductSpace");
}

TEST(Concepts, NormedSpace) {
    static_assert(NormedSpace<Vec3d, double>, "Vec3d must be NormedSpace");
}

// ============================================================================
// 值区间测试 / ValueRange tests
// ============================================================================

TEST(ValueRange, ClosedRange) {
    auto range = ValueRange<double>::closed(1.0, 10.0);
    EXPECT_TRUE(range.contains(1.0));
    EXPECT_TRUE(range.contains(5.0));
    EXPECT_TRUE(range.contains(10.0));
    EXPECT_FALSE(range.contains(0.9));
    EXPECT_FALSE(range.contains(10.1));
    EXPECT_FALSE(range.is_empty());
}

TEST(ValueRange, OpenRange) {
    auto range = ValueRange<double>::open(1.0, 10.0);
    EXPECT_FALSE(range.contains(1.0));
    EXPECT_TRUE(range.contains(5.0));
    EXPECT_FALSE(range.contains(10.0));
    EXPECT_TRUE(range.contains(1.1));
    EXPECT_TRUE(range.contains(9.9));
}

TEST(ValueRange, LeftClosedRightOpen) {
    auto range = ValueRange<double>::left_closed_right_open(1.0, 10.0);
    EXPECT_TRUE(range.contains(1.0));
    EXPECT_FALSE(range.contains(10.0));
    EXPECT_TRUE(range.contains(9.9));
}

TEST(ValueRange, EmptyRange) {
    auto range = ValueRange<double>::open(5.0, 5.0);
    EXPECT_TRUE(range.is_empty());
}

TEST(ValueRange, Length) {
    auto range = ValueRange<double>::closed(1.0, 10.0);
    auto len = range.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 9.0);
}

TEST(ValueRange, InfiniteRange) {
    ValueRange<double, Closed, Open> range(
        Bound<double, Closed>(ValueWrapper<double>::finite(0.0), Closed{}),
        Bound<double, Open>(ValueWrapper<double>::positive_infinity(), Open{})
    );
    EXPECT_TRUE(range.contains(0.0));
    EXPECT_TRUE(range.contains(1000.0));
    EXPECT_FALSE(range.contains(-1.0));
    EXPECT_FALSE(range.length().has_value());
}

// ============================================================================
// ValueWrapper 测试 / ValueWrapper tests
// ============================================================================

TEST(ValueWrapper, Finite) {
    auto v = ValueWrapper<double>::finite(42.0);
    EXPECT_TRUE(v.is_finite());
    EXPECT_FALSE(v.is_positive_infinity());
    EXPECT_FALSE(v.is_negative_infinity());
    EXPECT_DOUBLE_EQ(v.value(), 42.0);
}

TEST(ValueWrapper, PositiveInfinity) {
    auto v = ValueWrapper<double>::positive_infinity();
    EXPECT_FALSE(v.is_finite());
    EXPECT_TRUE(v.is_positive_infinity());
    EXPECT_FALSE(v.is_negative_infinity());
}

TEST(ValueWrapper, NegativeInfinity) {
    auto v = ValueWrapper<double>::negative_infinity();
    EXPECT_FALSE(v.is_finite());
    EXPECT_FALSE(v.is_positive_infinity());
    EXPECT_TRUE(v.is_negative_infinity());
}

TEST(ValueWrapper, Comparison) {
    auto a = ValueWrapper<double>::finite(1.0);
    auto b = ValueWrapper<double>::finite(2.0);
    auto pos_inf = ValueWrapper<double>::positive_infinity();
    auto neg_inf = ValueWrapper<double>::negative_infinity();

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(neg_inf < a);
    EXPECT_TRUE(a < pos_inf);
    EXPECT_FALSE(pos_inf < a);
    EXPECT_FALSE(neg_inf < neg_inf);
}

// ============================================================================
// NumericTraits 测试 / Numeric traits tests
// ============================================================================

TEST(NumericTraits, Basic) {
    EXPECT_DOUBLE_EQ(NumericTraits<double>::zero(), 0.0);
    EXPECT_DOUBLE_EQ(NumericTraits<double>::one(), 1.0);
    EXPECT_GT(NumericTraits<double>::epsilon(), 0.0);
    EXPECT_LT(NumericTraits<double>::min_value(), 0.0);
    EXPECT_GT(NumericTraits<double>::max_value(), 0.0);
}

TEST(NumericTraits, Infinity) {
    EXPECT_TRUE(std::isinf(NumericTraits<double>::positive_infinity()));
    EXPECT_TRUE(std::isinf(NumericTraits<double>::negative_infinity()));
    EXPECT_GT(NumericTraits<double>::positive_infinity(), 0.0);
    EXPECT_LT(NumericTraits<double>::negative_infinity(), 0.0);
}

TEST(NumericTraits, Pi) {
    EXPECT_NEAR(NumericTraits<double>::pi(), 3.14159265358979, 1e-10);
}

TEST(NumericTraits, IsNan) {
    EXPECT_FALSE(NumericTraits<double>::is_nan(1.0));
    EXPECT_TRUE(NumericTraits<double>::is_nan(std::nan("")));
}

TEST(NumericTraits, IsInf) {
    EXPECT_FALSE(NumericTraits<double>::is_inf(1.0));
    EXPECT_TRUE(NumericTraits<double>::is_inf(std::numeric_limits<double>::infinity()));
}

// ============================================================================
// 模运算测试 / Modular arithmetic tests
// ============================================================================

TEST(ModArithmetic, Gcd) {
    EXPECT_EQ(gcd(12, 8), 4);
    EXPECT_EQ(gcd(7, 5), 1);
    EXPECT_EQ(gcd(0, 5), 5);
    EXPECT_EQ(gcd(5, 0), 5);
}

TEST(ModArithmetic, Lcm) {
    EXPECT_EQ(lcm(4, 6), 12);
    EXPECT_EQ(lcm(3, 5), 15);
}

// ============================================================================
// 向量测试 / Vector tests
// ============================================================================

TEST(Vector, Construction) {
    Vec3d v = {1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_EQ(v.size(), 3u);
}

TEST(Vector, Zero) {
    auto v = Vec3d::zero();
    EXPECT_DOUBLE_EQ(v[0], 0.0);
    EXPECT_DOUBLE_EQ(v[1], 0.0);
    EXPECT_DOUBLE_EQ(v[2], 0.0);
}

TEST(Vector, Addition) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {4.0, 5.0, 6.0};
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c[0], 5.0);
    EXPECT_DOUBLE_EQ(c[1], 7.0);
    EXPECT_DOUBLE_EQ(c[2], 9.0);
}

TEST(Vector, Subtraction) {
    Vec3d a = {4.0, 5.0, 6.0};
    Vec3d b = {1.0, 2.0, 3.0};
    auto c = a - b;
    EXPECT_DOUBLE_EQ(c[0], 3.0);
    EXPECT_DOUBLE_EQ(c[1], 3.0);
    EXPECT_DOUBLE_EQ(c[2], 3.0);
}

TEST(Vector, ScalarMultiplication) {
    Vec3d v = {1.0, 2.0, 3.0};
    auto r = v * 2.0;
    EXPECT_DOUBLE_EQ(r[0], 2.0);
    EXPECT_DOUBLE_EQ(r[1], 4.0);
    EXPECT_DOUBLE_EQ(r[2], 6.0);

    auto r2 = 3.0 * v;
    EXPECT_DOUBLE_EQ(r2[0], 3.0);
    EXPECT_DOUBLE_EQ(r2[1], 6.0);
    EXPECT_DOUBLE_EQ(r2[2], 9.0);
}

TEST(Vector, DotProduct) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 32.0);  // 1*4 + 2*5 + 3*6 = 32
}

TEST(Vector, Norm) {
    Vec3d v = {3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
    EXPECT_DOUBLE_EQ(v.norm_squared(), 25.0);
}

TEST(Vector, Normalize) {
    Vec3d v = {3.0, 4.0, 0.0};
    auto n = v.normalized();
    EXPECT_NEAR(n.norm(), 1.0, 1e-10);
    EXPECT_NEAR(n[0], 0.6, 1e-10);
    EXPECT_NEAR(n[1], 0.8, 1e-10);
}

TEST(Vector, Equality) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {1.0, 2.0, 3.0};
    Vec3d c = {1.0, 2.0, 4.0};
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

// ============================================================================
// 矩阵测试 / Matrix tests
// ============================================================================

TEST(Matrix, Construction) {
    Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.at(0, 1), 2.0);
    EXPECT_DOUBLE_EQ(m.at(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 4.0);
}

TEST(Matrix, Zero) {
    auto m = Mat2d::zero();
    EXPECT_DOUBLE_EQ(m.at(0, 0), 0.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 0.0);
}

TEST(Matrix, Identity) {
    auto m = Mat3d::identity();
    EXPECT_DOUBLE_EQ(m.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m.at(1, 1), 1.0);
    EXPECT_DOUBLE_EQ(m.at(2, 2), 1.0);
    EXPECT_DOUBLE_EQ(m.at(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(m.at(1, 0), 0.0);
}

TEST(Matrix, Addition) {
    Mat2d a(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    Mat2d b(std::array<std::array<double, 2>, 2>{{{5.0, 6.0}, {7.0, 8.0}}});
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c.at(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(c.at(0, 1), 8.0);
    EXPECT_DOUBLE_EQ(c.at(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(c.at(1, 1), 12.0);
}

TEST(Matrix, ScalarMultiplication) {
    Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto r = m * 2.0;
    EXPECT_DOUBLE_EQ(r.at(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(r.at(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(r.at(1, 0), 6.0);
    EXPECT_DOUBLE_EQ(r.at(1, 1), 8.0);
}

TEST(Matrix, Multiplication) {
    Mat2d a(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    Mat2d b(std::array<std::array<double, 2>, 2>{{{5.0, 6.0}, {7.0, 8.0}}});
    auto c = a * b;
    // [1*5+2*7, 1*6+2*8] = [19, 22]
    // [3*5+4*7, 3*6+4*8] = [43, 50]
    EXPECT_DOUBLE_EQ(c.at(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(c.at(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(c.at(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(c.at(1, 1), 50.0);
}

TEST(Matrix, MatrixVectorMultiplication) {
    Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    Vec2d v = {5.0, 6.0};
    auto r = m * v;
    // [1*5+2*6] = [17]
    // [3*5+4*6] = [39]
    EXPECT_DOUBLE_EQ(r[0], 17.0);
    EXPECT_DOUBLE_EQ(r[1], 39.0);
}

TEST(Matrix, Transpose) {
    Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    auto t = m.transposed();
    EXPECT_DOUBLE_EQ(t.at(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(t.at(0, 1), 3.0);
    EXPECT_DOUBLE_EQ(t.at(1, 0), 2.0);
    EXPECT_DOUBLE_EQ(t.at(1, 1), 4.0);
}

TEST(Matrix, Determinant2x2) {
    Mat2d m(std::array<std::array<double, 2>, 2>{{{1.0, 2.0}, {3.0, 4.0}}});
    EXPECT_DOUBLE_EQ(m.determinant(), -2.0);  // 1*4 - 2*3 = -2
}

TEST(Matrix, Determinant3x3) {
    Mat3d m(std::array<std::array<double, 3>, 3>{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}});
    EXPECT_DOUBLE_EQ(m.determinant(), 0.0);  // singular matrix
}

TEST(Matrix, IdentityMultiplication) {
    auto I = Mat3d::identity();
    Mat3d m(std::array<std::array<double, 3>, 3>{{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}});
    auto r = I * m;
    EXPECT_EQ(r, m);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, VectorMatrixWorkflow) {
    // 创建变换矩阵（缩放 2x）
    Mat3d scale(std::array<std::array<double, 3>, 3>{{{2.0, 0.0, 0.0}, {0.0, 2.0, 0.0}, {0.0, 0.0, 2.0}}});

    // 创建向量
    Vec3d v = {1.0, 2.0, 3.0};

    // 变换
    auto result = scale * v;
    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], 4.0);
    EXPECT_DOUBLE_EQ(result[2], 6.0);

    // 验证范数关系
    EXPECT_DOUBLE_EQ(result.norm(), 2.0 * v.norm());
}
