/// ospf_quantities 测试
/// 覆盖维度、单位、物理量算术、单位换算、混算防护。

#include <gtest/gtest.h>

#include <ospf/quantities/dimension.hpp>
#include <ospf/quantities/unit.hpp>
#include <ospf/quantities/quantity.hpp>

using namespace ospf::quantities;

// ============================================================================
// Dimension 测试 / Dimension tests
// ============================================================================

TEST(Dimension, Basic) {
    EXPECT_EQ(DIM_LENGTH, DIM_LENGTH);
    EXPECT_NE(DIM_LENGTH, DIM_MASS);
    EXPECT_TRUE(DIMENSIONLESS.is_dimensionless());
    EXPECT_FALSE(DIM_LENGTH.is_dimensionless());
}

TEST(Dimension, Multiplication) {
    auto area = DIM_LENGTH * DIM_LENGTH;
    EXPECT_EQ(area, DIM_AREA);

    auto force = DIM_MASS * DIM_LENGTH / (DIM_TIME * DIM_TIME);
    EXPECT_EQ(force, DIM_FORCE);
}

TEST(Dimension, Division) {
    auto velocity = DIM_LENGTH / DIM_TIME;
    EXPECT_EQ(velocity, DIM_VELOCITY);
}

TEST(Dimension, Power) {
    auto area = DIM_LENGTH.pow(2);
    EXPECT_EQ(area, DIM_AREA);

    auto volume = DIM_LENGTH.pow(3);
    EXPECT_EQ(volume, DIM_VOLUME);
}

// ============================================================================
// Unit 测试 / Unit tests
// ============================================================================

TEST(Unit, Basic) {
    EXPECT_EQ(UNIT_METER.dimension, DIM_LENGTH);
    EXPECT_EQ(UNIT_METER.name, "meter");
    EXPECT_EQ(UNIT_METER.symbol, "m");
}

TEST(Unit, Conversion) {
    EXPECT_DOUBLE_EQ(convert(1.0, UNIT_KILOMETER, UNIT_METER), 1000.0);
    EXPECT_DOUBLE_EQ(convert(1000.0, UNIT_METER, UNIT_KILOMETER), 1.0);
    EXPECT_DOUBLE_EQ(convert(1.0, UNIT_CENTIMETER, UNIT_METER), 0.01);
    EXPECT_DOUBLE_EQ(convert(1.0, UNIT_HOUR, UNIT_SECOND), 3600.0);
    EXPECT_DOUBLE_EQ(convert(1.0, UNIT_GRAM, UNIT_KILOGRAM), 0.001);
}

// ============================================================================
// Quantity 测试 / Quantity tests
// ============================================================================

TEST(Quantity, Construction) {
    Length l(5.0);
    EXPECT_DOUBLE_EQ(l.value(), 5.0);
    EXPECT_EQ(l.dimension(), DIM_LENGTH);
}

TEST(Quantity, Addition) {
    Length a(3.0);
    Length b(4.0);
    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum.value(), 7.0);
}

TEST(Quantity, Subtraction) {
    Length a(7.0);
    Length b(3.0);
    auto diff = a - b;
    EXPECT_DOUBLE_EQ(diff.value(), 4.0);
}

TEST(Quantity, ScalarMultiplication) {
    Length l(5.0);
    auto r = l * 2.0;
    EXPECT_DOUBLE_EQ(r.value(), 10.0);

    auto r2 = 3.0 * l;
    EXPECT_DOUBLE_EQ(r2.value(), 15.0);
}

TEST(Quantity, ScalarDivision) {
    Length l(10.0);
    auto r = l / 2.0;
    EXPECT_DOUBLE_EQ(r.value(), 5.0);
}

TEST(Quantity, SameDimensionDivision) {
    Length a(10.0);
    Length b(2.0);
    double ratio = a / b;
    EXPECT_DOUBLE_EQ(ratio, 5.0);
}

TEST(Quantity, Comparison) {
    Length a(3.0);
    Length b(5.0);
    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a >= a);
    EXPECT_TRUE(a == a);
}

TEST(Quantity, Negation) {
    Length l(5.0);
    auto neg = -l;
    EXPECT_DOUBLE_EQ(neg.value(), -5.0);
}

TEST(Quantity, Velocity) {
    Velocity v(10.0);
    EXPECT_DOUBLE_EQ(v.value(), 10.0);
    EXPECT_EQ(v.dimension(), DIM_VELOCITY);
}

TEST(Quantity, Force) {
    Force f(100.0);
    EXPECT_DOUBLE_EQ(f.value(), 100.0);
    EXPECT_EQ(f.dimension(), DIM_FORCE);
}

// ============================================================================
// 单位混算防护测试 / Unit mismatch protection tests
// ============================================================================

TEST(Quantity, DifferentDimensionsAreDifferentTypes) {
    Length l(1.0);
    Mass m(2.0);

    // 不同维度是不同的模板实例化
    static_assert(!std::is_same_v<decltype(l), decltype(m)>,
        "Length and Mass must be different types");

    // 各自可以独立运算
    auto l2 = l + l;
    EXPECT_DOUBLE_EQ(l2.value(), 2.0);

    auto m2 = m + m;
    EXPECT_DOUBLE_EQ(m2.value(), 4.0);

    // 编译期防护：以下代码不会编译
    // auto bad = l + m;  // COMPILE ERROR: different template instantiations
}

TEST(Quantity, SameDimensionDifferentScale) {
    // 同维度不同比例可以通过换算
    Length meters(1000.0);
    Length kilometers = convert_unit(meters, 1.0, 1000.0);
    EXPECT_DOUBLE_EQ(kilometers.value(), 1.0);
}

// ============================================================================
// QuantityVector 测试 / QuantityVector tests
// ============================================================================

TEST(QuantityVector, Basic) {
    QuantityVector<double, DIM_LENGTH, 3> v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
    EXPECT_EQ(v.size(), 3u);
}

TEST(QuantityVector, Addition) {
    QuantityVector<double, DIM_LENGTH, 2> a(1.0, 2.0);
    QuantityVector<double, DIM_LENGTH, 2> b(3.0, 4.0);
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c[0], 4.0);
    EXPECT_DOUBLE_EQ(c[1], 6.0);
}

TEST(QuantityVector, ScalarMultiplication) {
    QuantityVector<double, DIM_LENGTH, 2> v(1.0, 2.0);
    auto r = v * 3.0;
    EXPECT_DOUBLE_EQ(r[0], 3.0);
    EXPECT_DOUBLE_EQ(r[1], 6.0);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, PhysicsWorkflow) {
    // F = m * a
    Mass m(10.0);
    Acceleration a(9.81);
    double force_value = m.value() * a.value();
    Force f(force_value);
    EXPECT_NEAR(f.value(), 98.1, 1e-10);

    // 单位换算
    Length distance(1000.0);
    Length km = convert_unit(distance, 1.0, 1000.0);
    EXPECT_DOUBLE_EQ(km.value(), 1.0);

    // 时间换算
    Time t(3600.0);
    Time hours = convert_unit(t, 1.0, 3600.0);
    EXPECT_DOUBLE_EQ(hours.value(), 1.0);
}

TEST(Integration, DimensionArithmetic) {
    // 验证能量维度 = 质量 * 速度^2
    // [E] = M * (L/T)^2 = M * L^2 / T^2
    auto energy_dim = DIM_MASS * DIM_LENGTH * DIM_LENGTH / (DIM_TIME * DIM_TIME);
    EXPECT_EQ(energy_dim, DIM_ENERGY);

    // 验证功率 = 能量 / 时间
    auto power_dim = DIM_ENERGY / DIM_TIME;
    EXPECT_EQ(power_dim, DIM_POWER);
}
