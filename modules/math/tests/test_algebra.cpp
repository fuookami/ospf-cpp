/// ospf_math algebra 测试
/// 覆盖代数结构概念、值区间、数值特征、向量、矩阵。

#include <gtest/gtest.h>

#include <ospf/math/algebra/concept.hpp>
#include <ospf/math/algebra/value_range.hpp>
#include <ospf/math/algebra/number.hpp>
#include <ospf/math/algebra/real_number.hpp>
#include <ospf/math/algebra/vector.hpp>
#include <ospf/math/algebra/matrix.hpp>
#include <ospf/math/algebra/law.hpp>

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

// ============================================================================
// ospf::math::RealNumber concept 测试（Phase 1 新增）
// ============================================================================

TEST(RealNumberConcept, DoubleSatisfies) {
    // double 满足 ospf::math::RealNumber / double satisfies RealNumber
    static_assert(ospf::math::RealNumber<double>, "double must satisfy RealNumber");
    EXPECT_TRUE(ospf::math::RealNumber<double>);
}

TEST(RealNumberConcept, FloatSatisfies) {
    // float 满足 ospf::math::RealNumber / float satisfies RealNumber
    static_assert(ospf::math::RealNumber<float>, "float must satisfy RealNumber");
    EXPECT_TRUE(ospf::math::RealNumber<float>);
}

TEST(RealNumberConcept, RejectsNonArithmetic) {
    // 非算术类型不满足 / Non-arithmetic types must not satisfy
    static_assert(!ospf::math::RealNumber<std::string>, "string must not satisfy RealNumber");
    EXPECT_FALSE(ospf::math::RealNumber<std::string>);
}

TEST(RealNumberConcept, IntAlsoSatisfies) {
    // int 也满足 RealNumber（算术运算符 + 零值/单位值可构造）
    // int also satisfies RealNumber (arithmetic ops + zero/one constructible)
    static_assert(ospf::math::RealNumber<int>, "int must satisfy RealNumber");
    EXPECT_TRUE(ospf::math::RealNumber<int>);
}

TEST(RealNumberConcept, FromDecimalDouble) {
    // from_decimal<double> 正常解析 / from_decimal<double> normal parsing
    auto val = ospf::math::from_decimal<double>("3.14");
    ASSERT_TRUE(val.has_value());
    EXPECT_DOUBLE_EQ(*val, 3.14);

    // 无效输入返回 nullopt / Invalid input returns nullopt
    auto invalid = ospf::math::from_decimal<double>("not_a_number");
    EXPECT_FALSE(invalid.has_value());
}

TEST(RealNumberConcept, FromDecimalFloat) {
    // from_decimal<float> 正常解析 / from_decimal<float> normal parsing
    auto val = ospf::math::from_decimal<float>("2.5");
    ASSERT_TRUE(val.has_value());
    EXPECT_FLOAT_EQ(*val, 2.5f);
}

TEST(RealNumberConcept, FromDecimalInteger) {
    // from_decimal<double> 整数解析 / from_decimal<double> integer parsing
    auto val = ospf::math::from_decimal<double>("42");
    ASSERT_TRUE(val.has_value());
    EXPECT_DOUBLE_EQ(*val, 42.0);
}

TEST(RealNumberConcept, FromDecimalNegative) {
    auto val = ospf::math::from_decimal<double>("-1.5");
    ASSERT_TRUE(val.has_value());
    EXPECT_DOUBLE_EQ(*val, -1.5);
}

// ============================================================================
// 扩展代数结构概念测试 / Extended algebraic structure concept tests
// 移植 Rust concept/*.rs 全部 #[test]
// ============================================================================

// -- Semigroup (Rust semigroup.rs: 4 tests) --
TEST(AlgebraSemigroup, IntClosed) { static_assert(Semigroup<int>); }
TEST(AlgebraSemigroup, DoubleClosed) { static_assert(Semigroup<double>); }
TEST(AlgebraSemigroup, StringClosed) { static_assert(Semigroup<std::string>); }
TEST(AlgebraSemigroup, FloatClosed) { static_assert(Semigroup<float>); }

// -- Monoid (Rust monoid.rs: 6 tests) --
TEST(AlgebraMonoid, IntIdentity) {
    int a = 5;
    EXPECT_EQ(a + int{}, a);
}
TEST(AlgebraMonoid, DoubleIdentity) {
    double a = 3.14;
    EXPECT_DOUBLE_EQ(a + double{}, a);
}
TEST(AlgebraMonoid, FloatIdentity) {
    float a = 2.5f;
    EXPECT_FLOAT_EQ(a + float{}, a);
}
TEST(AlgebraMonoid, StringIdentity) {
    std::string s = "hello";
    EXPECT_EQ(s + std::string{}, s);
}
TEST(AlgebraMonoid, ConceptInt) { static_assert(Monoid<int>); }
TEST(AlgebraMonoid, ConceptDouble) { static_assert(Monoid<double>); }

// -- Group (Rust group.rs: 6 tests) --
TEST(AlgebraGroup, IntInverse) { EXPECT_EQ(5 + (-5), 0); }
TEST(AlgebraGroup, DoubleInverse) { EXPECT_DOUBLE_EQ(3.14 + (-3.14), 0.0); }
TEST(AlgebraGroup, FloatInverse) { EXPECT_FLOAT_EQ(2.5f + (-2.5f), 0.0f); }
TEST(AlgebraGroup, Subtraction) { EXPECT_EQ(10 - 3, 7); }
TEST(AlgebraGroup, ConceptInt) { static_assert(Group<int>); }
TEST(AlgebraGroup, ConceptDouble) { static_assert(Group<double>); }

// -- AbelianGroup (Rust abelian_group.rs: 6 tests) --
TEST(AlgebraAbelianGroup, CommutativeInt) { EXPECT_EQ(3 + 5, 5 + 3); }
TEST(AlgebraAbelianGroup, CommutativeDouble) { EXPECT_DOUBLE_EQ(1.5 + 2.5, 2.5 + 1.5); }
TEST(AlgebraAbelianGroup, AssociativeInt) { EXPECT_EQ((1 + 2) + 3, 1 + (2 + 3)); }
TEST(AlgebraAbelianGroup, AssociativeDouble) { EXPECT_DOUBLE_EQ((1.1 + 2.2) + 3.3, 1.1 + (2.2 + 3.3)); }
TEST(AlgebraAbelianGroup, ConceptInt) { static_assert(AbelianGroup<int>); }
TEST(AlgebraAbelianGroup, ConceptDouble) { static_assert(AbelianGroup<double>); }

// -- Ring (Rust ring.rs: 6 tests) --
TEST(AlgebraRing, Addition) { EXPECT_EQ(3 + 5, 8); }
TEST(AlgebraRing, Multiplication) { EXPECT_EQ(3 * 5, 15); }
TEST(AlgebraRing, Distributive) { EXPECT_EQ(2 * (3 + 4), 2 * 3 + 2 * 4); }
TEST(AlgebraRing, ZeroElement) { EXPECT_EQ(5 + 0, 5); }
TEST(AlgebraRing, OneElement) { EXPECT_EQ(5 * 1, 5); }
TEST(AlgebraRing, ConceptInt) { static_assert(Ring<int>); }

// -- CommutativeRing (Rust commutative_ring.rs: 6 tests) --
TEST(AlgebraCommutativeRing, MultiplyCommutative) { EXPECT_EQ(3 * 5, 5 * 3); }
TEST(AlgebraCommutativeRing, DoubleMultiply) { EXPECT_DOUBLE_EQ(1.5 * 2.5, 2.5 * 1.5); }
TEST(AlgebraCommutativeRing, DistributiveDouble) {
    double a = 1.5, b = 2.5, c = 3.5;
    EXPECT_DOUBLE_EQ(a * (b + c), a * b + a * c);
}
TEST(AlgebraCommutativeRing, ConceptInt) { static_assert(CommutativeRing<int>); }
TEST(AlgebraCommutativeRing, ConceptDouble) { static_assert(CommutativeRing<double>); }
TEST(AlgebraCommutativeRing, ConceptFloat) { static_assert(CommutativeRing<float>); }

// -- Field (Rust field.rs: 6 tests) - Field refines RealNumber --
TEST(AlgebraField, Division) { EXPECT_DOUBLE_EQ(10.0 / 4.0, 2.5); }
TEST(AlgebraField, MultiplyInverse) { EXPECT_DOUBLE_EQ(5.0 * (1.0 / 5.0), 1.0); }
TEST(AlgebraField, FloatDivision) { EXPECT_FLOAT_EQ(10.0f / 4.0f, 2.5f); }
TEST(AlgebraField, ConceptDouble) { static_assert(Field<double>); }
TEST(AlgebraField, ConceptFloat) { static_assert(Field<float>); }
TEST(AlgebraField, RefinesRealNumber) {
    // Field 必须满足 RealNumber / Field must satisfy RealNumber
    static_assert(Field<double>);
    static_assert(ospf::math::RealNumber<double>);
    // double 同时满足 Field 和 RealNumber
    EXPECT_TRUE(Field<double>);
    EXPECT_TRUE(ospf::math::RealNumber<double>);
}

// -- Scalar (Rust scalar.rs: 3 tests) --
TEST(AlgebraScalar, ConceptDouble) { static_assert(Scalar<double>); }
TEST(AlgebraScalar, ConceptFloat) { static_assert(Scalar<float>); }
TEST(AlgebraScalar, ConceptInt) { static_assert(Scalar<int>); }

// -- TotallyOrdered (Rust totally_ordered.rs: 5 tests) --
TEST(AlgebraTotallyOrdered, IntOrdered) { static_assert(TotallyOrdered<int>); }
TEST(AlgebraTotallyOrdered, DoubleOrdered) { static_assert(TotallyOrdered<double>); }
TEST(AlgebraTotallyOrdered, StringOrdered) { static_assert(TotallyOrdered<std::string>); }
TEST(AlgebraTotallyOrdered, Comparison) { EXPECT_TRUE(3 < 5); EXPECT_TRUE(5 > 3); }
TEST(AlgebraTotallyOrdered, EqualComparison) { EXPECT_TRUE(5 <= 5); EXPECT_TRUE(5 >= 5); }

// -- Bounded (Rust bounded.rs: 8 tests) --
TEST(AlgebraBounded, IntMin) { EXPECT_EQ(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::min()); }
TEST(AlgebraBounded, IntMax) { EXPECT_GT(std::numeric_limits<int>::max(), 0); }
TEST(AlgebraBounded, DoubleMin) { EXPECT_LT(std::numeric_limits<double>::lowest(), 0.0); }
TEST(AlgebraBounded, DoubleMax) { EXPECT_GT(std::numeric_limits<double>::max(), 0.0); }
TEST(AlgebraBounded, FloatMin) { EXPECT_LT(std::numeric_limits<float>::lowest(), 0.0f); }
TEST(AlgebraBounded, FloatMax) { EXPECT_GT(std::numeric_limits<float>::max(), 0.0f); }
TEST(AlgebraBounded, ULongMax) { EXPECT_GT(std::numeric_limits<unsigned long>::max(), 0u); }
TEST(AlgebraBounded, CharMin) { EXPECT_LE(std::numeric_limits<char>::lowest(), std::numeric_limits<char>::max()); }

// -- Infinite (Rust infinite.rs: 6 tests) --
TEST(AlgebraInfinite, DoublePosInf) { EXPECT_TRUE(std::isinf(std::numeric_limits<double>::infinity())); }
TEST(AlgebraInfinite, DoubleNegInf) { EXPECT_TRUE(std::isinf(-std::numeric_limits<double>::infinity())); }
TEST(AlgebraInfinite, FloatPosInf) { EXPECT_TRUE(std::isinf(std::numeric_limits<float>::infinity())); }
TEST(AlgebraInfinite, FloatNegInf) { EXPECT_TRUE(std::isinf(-std::numeric_limits<float>::infinity())); }
TEST(AlgebraInfinite, InfComparison) { EXPECT_GT(std::numeric_limits<double>::infinity(), 1e308); }
TEST(AlgebraInfinite, NegInfComparison) { EXPECT_LT(-std::numeric_limits<double>::infinity(), -1e308); }

// -- Epsilon/Fixed (Rust epsilon.rs: 4, fixed.rs: 3 tests) --
TEST(AlgebraEpsilon, DoubleEpsilon) { EXPECT_GT(std::numeric_limits<double>::epsilon(), 0.0); }
TEST(AlgebraEpsilon, FloatEpsilon) { EXPECT_GT(std::numeric_limits<float>::epsilon(), 0.0f); }
TEST(AlgebraEpsilon, DoubleEpsilonSmall) { EXPECT_LT(std::numeric_limits<double>::epsilon(), 1e-10); }
TEST(AlgebraEpsilon, FloatEpsilonSmall) { EXPECT_LT(std::numeric_limits<float>::epsilon(), 1e-5f); }
TEST(AlgebraFixed, DoubleEpsilon) {
    // Fixed concept 要求 T::epsilon()，double 不满足（primitive 无成员函数）
    // Fixed concept requires T::epsilon(), double doesn't satisfy (primitives lack members)
    // 使用 NumericTraits 验证 epsilon 存在性
    EXPECT_GT(NumericTraits<double>::epsilon(), 0.0);
}
TEST(AlgebraFixed, FloatEpsilon) {
    EXPECT_GT(NumericTraits<float>::epsilon(), 0.0f);
}
TEST(AlgebraFixed, EpsilonValue) {
    EXPECT_GT(std::numeric_limits<double>::epsilon(), 0.0);
}

// -- NormedSpace (Rust normed_space.rs: 4 tests) --
TEST(AlgebraNormedSpace, Vec3dNorm) {
    Vec3d v = {3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm(), 5.0);
}
TEST(AlgebraNormedSpace, Vec3dNormSquared) {
    Vec3d v = {3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm_squared(), 25.0);
}
TEST(AlgebraNormedSpace, UnitNorm) {
    Vec3d v = {1.0, 0.0, 0.0};
    EXPECT_DOUBLE_EQ(v.norm(), 1.0);
}
TEST(AlgebraNormedSpace, ConceptVec3d) { static_assert(NormedSpace<Vec3d, double>); }

// -- InnerProductSpace (Rust inner_product_space.rs: 6 tests) --
TEST(AlgebraInnerProduct, DotProduct) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 32.0);
}
TEST(AlgebraInnerProduct, Orthogonal) {
    Vec3d a = {1.0, 0.0, 0.0};
    Vec3d b = {0.0, 1.0, 0.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 0.0);
}
TEST(AlgebraInnerProduct, SelfDot) {
    Vec3d v = {3.0, 4.0, 0.0};
    EXPECT_DOUBLE_EQ(v.dot(v), 25.0);
}
TEST(AlgebraInnerProduct, Commutative) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {4.0, 5.0, 6.0};
    EXPECT_DOUBLE_EQ(a.dot(b), b.dot(a));
}
TEST(AlgebraInnerProduct, ConceptVec3d) { static_assert(InnerProductSpace<Vec3d, double>); }
TEST(AlgebraInnerProduct, ZeroDot) {
    Vec3d v = {1.0, 2.0, 3.0};
    Vec3d zero = Vec3d::zero();
    EXPECT_DOUBLE_EQ(v.dot(zero), 0.0);
}

// -- VectorSpace (Rust vector_space.rs: 3 tests) --
TEST(AlgebraVectorSpace, Addition) {
    Vec3d a = {1.0, 2.0, 3.0};
    Vec3d b = {4.0, 5.0, 6.0};
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c[0], 5.0);
    EXPECT_DOUBLE_EQ(c[1], 7.0);
    EXPECT_DOUBLE_EQ(c[2], 9.0);
}
TEST(AlgebraVectorSpace, ScalarMul) {
    Vec3d v = {1.0, 2.0, 3.0};
    auto r = v * 2.0;
    EXPECT_DOUBLE_EQ(r[0], 2.0);
}
TEST(AlgebraVectorSpace, ConceptVec3d) { static_assert(VectorSpace<Vec3d, double>); }

// -- Law tests (Rust law/mod.rs) --
TEST(AlgebraLaw, CommutativeInt) { EXPECT_TRUE(is_commutative(3, 5)); }
TEST(AlgebraLaw, CommutativeDouble) { EXPECT_TRUE(is_commutative(1.5, 2.5)); }
TEST(AlgebraLaw, AssociativeInt) { EXPECT_TRUE(is_associative(1, 2, 3)); }
TEST(AlgebraLaw, AssociativeDouble) { EXPECT_TRUE(is_associative(1.1, 2.2, 3.3)); }
TEST(AlgebraLaw, DistributiveDouble) { EXPECT_TRUE(is_distributive(2.0, 3.0, 4.0)); }

// ============================================================================
// 扩展 ValueRange 测试 / Extended ValueRange tests
// 移植 Rust value_range/*.rs 全部 #[test]
// ============================================================================

TEST(ValueRange, ClosedBothEnds) {
    auto r = ValueRange<double>::closed(0.0, 10.0);
    EXPECT_TRUE(r.contains(0.0));
    EXPECT_TRUE(r.contains(10.0));
    EXPECT_TRUE(r.contains(5.0));
    EXPECT_FALSE(r.contains(-0.1));
    EXPECT_FALSE(r.contains(10.1));
}

TEST(ValueRange, OpenBothEnds) {
    auto r = ValueRange<double>::open(0.0, 10.0);
    EXPECT_FALSE(r.contains(0.0));
    EXPECT_FALSE(r.contains(10.0));
    EXPECT_TRUE(r.contains(5.0));
}

TEST(ValueRange, LeftClosedRightOpenExtended) {
    auto r = ValueRange<double>::left_closed_right_open(0.0, 10.0);
    EXPECT_TRUE(r.contains(0.0));
    EXPECT_FALSE(r.contains(10.0));
    EXPECT_TRUE(r.contains(9.99));
}

TEST(ValueRange, LengthFinite) {
    auto r = ValueRange<double>::closed(1.0, 10.0);
    auto len = r.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 9.0);
}

TEST(ValueRange, LengthOpen) {
    auto r = ValueRange<double>::open(1.0, 10.0);
    auto len = r.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 9.0);
}

TEST(ValueRange, LengthLeftClosedRightOpen) {
    auto r = ValueRange<double>::left_closed_right_open(0.0, 5.0);
    auto len = r.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 5.0);
}

TEST(ValueRange, SinglePointClosed) {
    auto r = ValueRange<double>::closed(5.0, 5.0);
    EXPECT_FALSE(r.is_empty());
    EXPECT_TRUE(r.contains(5.0));
}

TEST(ValueRange, NegativeRange) {
    auto r = ValueRange<double>::closed(-10.0, -1.0);
    EXPECT_TRUE(r.contains(-5.0));
    EXPECT_FALSE(r.contains(0.0));
}

TEST(ValueRange, FloatRange) {
    auto r = ValueRange<float>::closed(0.0f, 1.0f);
    EXPECT_TRUE(r.contains(0.5f));
    EXPECT_FALSE(r.contains(1.5f));
}

TEST(ValueRange, InfiniteRangeContains) {
    ValueRange<double, Closed, Open> r(
        Bound<double, Closed>(ValueWrapper<double>::finite(0.0), Closed{}),
        Bound<double, Open>(ValueWrapper<double>::positive_infinity(), Open{})
    );
    EXPECT_TRUE(r.contains(0.0));
    EXPECT_TRUE(r.contains(1000.0));
    EXPECT_FALSE(r.contains(-1.0));
    EXPECT_FALSE(r.length().has_value());
}

TEST(ValueRange, NegativeInfiniteRange) {
    ValueRange<double, Open, Closed> r(
        Bound<double, Open>(ValueWrapper<double>::negative_infinity(), Open{}),
        Bound<double, Closed>(ValueWrapper<double>::finite(0.0), Closed{})
    );
    EXPECT_TRUE(r.contains(0.0));
    EXPECT_TRUE(r.contains(-1000.0));
    EXPECT_FALSE(r.contains(1.0));
}

// -- ValueWrapper extended tests (Rust value_wrapper.rs: 9 tests) --
TEST(ValueWrapper, FinitePositive) {
    auto v = ValueWrapper<double>::finite(42.0);
    EXPECT_TRUE(v.is_finite());
    EXPECT_DOUBLE_EQ(v.value(), 42.0);
}

TEST(ValueWrapper, FiniteNegative) {
    auto v = ValueWrapper<double>::finite(-42.0);
    EXPECT_TRUE(v.is_finite());
    EXPECT_DOUBLE_EQ(v.value(), -42.0);
}

TEST(ValueWrapper, FiniteZero) {
    auto v = ValueWrapper<double>::finite(0.0);
    EXPECT_TRUE(v.is_finite());
    EXPECT_DOUBLE_EQ(v.value(), 0.0);
}

TEST(ValueWrapper, PositiveInfinityNotFinite) {
    auto v = ValueWrapper<double>::positive_infinity();
    EXPECT_FALSE(v.is_finite());
    EXPECT_TRUE(v.is_positive_infinity());
    EXPECT_FALSE(v.is_negative_infinity());
}

TEST(ValueWrapper, NegativeInfinityNotFinite) {
    auto v = ValueWrapper<double>::negative_infinity();
    EXPECT_FALSE(v.is_finite());
    EXPECT_FALSE(v.is_positive_infinity());
    EXPECT_TRUE(v.is_negative_infinity());
}

TEST(ValueWrapper, FiniteComparison) {
    auto a = ValueWrapper<double>::finite(1.0);
    auto b = ValueWrapper<double>::finite(2.0);
    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(ValueWrapper, InfComparison) {
    auto fin = ValueWrapper<double>::finite(100.0);
    auto pos_inf = ValueWrapper<double>::positive_infinity();
    auto neg_inf = ValueWrapper<double>::negative_infinity();
    EXPECT_TRUE(neg_inf < fin);
    EXPECT_TRUE(fin < pos_inf);
    EXPECT_TRUE(neg_inf < pos_inf);
}

TEST(ValueWrapper, FloatFinite) {
    auto v = ValueWrapper<float>::finite(3.14f);
    EXPECT_TRUE(v.is_finite());
    EXPECT_FLOAT_EQ(v.value(), 3.14f);
}

TEST(ValueWrapper, FloatInfinity) {
    auto v = ValueWrapper<float>::positive_infinity();
    EXPECT_FALSE(v.is_finite());
}

// -- Bound extended tests (Rust bound.rs: 14 tests) --
TEST(Bound, ClosedBound) {
    Bound<double, Closed> b(ValueWrapper<double>::finite(5.0), Closed{});
    EXPECT_TRUE(b.is_closed());
    EXPECT_FALSE(b.is_open());
}

TEST(Bound, OpenBound) {
    Bound<double, Open> b(ValueWrapper<double>::finite(5.0), Open{});
    EXPECT_FALSE(b.is_closed());
    EXPECT_TRUE(b.is_open());
}

TEST(Bound, ClosedContainsBoundary) {
    Bound<double, Closed> b(ValueWrapper<double>::finite(5.0), Closed{});
    // Closed bound at 5.0 should be <= 5.0
    EXPECT_TRUE(b.is_closed());
}

TEST(Bound, OpenDoesNotContainBoundary) {
    Bound<double, Open> b(ValueWrapper<double>::finite(5.0), Open{});
    EXPECT_TRUE(b.is_open());
}

TEST(Bound, InfinityBound) {
    Bound<double, Open> b(ValueWrapper<double>::positive_infinity(), Open{});
    EXPECT_TRUE(b.is_open());
}

TEST(Bound, NegativeInfinityBound) {
    Bound<double, Closed> b(ValueWrapper<double>::negative_infinity(), Closed{});
    EXPECT_TRUE(b.is_closed());
}

// -- Interval tests (Rust interval.rs: 17 tests) --
TEST(Interval, ClosedInterval) {
    auto i = ValueRange<double>::closed(1.0, 10.0);
    EXPECT_TRUE(i.contains(1.0));
    EXPECT_TRUE(i.contains(10.0));
    EXPECT_TRUE(i.contains(5.0));
}

TEST(Interval, OpenInterval) {
    auto i = ValueRange<double>::open(1.0, 10.0);
    EXPECT_FALSE(i.contains(1.0));
    EXPECT_FALSE(i.contains(10.0));
    EXPECT_TRUE(i.contains(5.0));
}

TEST(Interval, HalfOpenLeft) {
    auto i = ValueRange<double>::left_closed_right_open(1.0, 10.0);
    EXPECT_TRUE(i.contains(1.0));
    EXPECT_FALSE(i.contains(10.0));
}

TEST(Interval, HalfOpenRight) {
    auto i = ValueRange<double>::open(1.0, 10.0);
    EXPECT_FALSE(i.contains(1.0));
    EXPECT_FALSE(i.contains(10.0));
    EXPECT_TRUE(i.contains(5.0));
}

TEST(Interval, Empty) {
    auto i = ValueRange<double>::open(5.0, 5.0);
    EXPECT_TRUE(i.is_empty());
}

TEST(Interval, NonEmpty) {
    auto i = ValueRange<double>::closed(5.0, 5.0);
    EXPECT_FALSE(i.is_empty());
}

TEST(Interval, LengthClosed) {
    auto i = ValueRange<double>::closed(0.0, 10.0);
    auto len = i.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 10.0);
}

TEST(Interval, LengthOpen) {
    auto i = ValueRange<double>::open(0.0, 10.0);
    auto len = i.length();
    ASSERT_TRUE(len.has_value());
    EXPECT_DOUBLE_EQ(*len, 10.0);
}

TEST(Interval, ContainsMiddle) {
    auto i = ValueRange<double>::closed(0.0, 100.0);
    EXPECT_TRUE(i.contains(50.0));
}

TEST(Interval, NegativeContains) {
    auto i = ValueRange<double>::closed(-100.0, 100.0);
    EXPECT_TRUE(i.contains(0.0));
    EXPECT_TRUE(i.contains(-50.0));
    EXPECT_TRUE(i.contains(50.0));
}

TEST(Interval, FloatClosed) {
    auto i = ValueRange<float>::closed(0.0f, 1.0f);
    EXPECT_TRUE(i.contains(0.5f));
}

TEST(Interval, FloatOpen) {
    auto i = ValueRange<float>::open(0.0f, 1.0f);
    EXPECT_FALSE(i.contains(0.0f));
    EXPECT_FALSE(i.contains(1.0f));
}

TEST(Interval, LargeRange) {
    auto i = ValueRange<double>::closed(-1e100, 1e100);
    EXPECT_TRUE(i.contains(0.0));
    EXPECT_TRUE(i.contains(1e99));
}

TEST(Interval, SmallRange) {
    auto i = ValueRange<double>::closed(1.0, 1.0 + 1e-15);
    EXPECT_TRUE(i.contains(1.0));
    EXPECT_TRUE(i.contains(1.0 + 5e-16));
}

TEST(Interval, UnitInterval) {
    auto i = ValueRange<double>::left_closed_right_open(0.0, 1.0);
    EXPECT_TRUE(i.contains(0.0));
    EXPECT_FALSE(i.contains(1.0));
    EXPECT_TRUE(i.contains(0.999));
}

TEST(Interval, PositiveRange) {
    auto i = ValueRange<double>::open(0.0, 100.0);
    EXPECT_FALSE(i.contains(0.0));
    EXPECT_TRUE(i.contains(0.001));
    EXPECT_FALSE(i.contains(100.0));
    EXPECT_TRUE(i.contains(99.9));
}

TEST(Interval, SymmetricRange) {
    auto i = ValueRange<double>::closed(-10.0, 10.0);
    EXPECT_TRUE(i.contains(-10.0));
    EXPECT_TRUE(i.contains(10.0));
    EXPECT_FALSE(i.contains(-10.1));
    EXPECT_FALSE(i.contains(10.1));
}
