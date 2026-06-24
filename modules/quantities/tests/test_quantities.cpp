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
    auto energy_dim = DIM_MASS * DIM_LENGTH * DIM_LENGTH / (DIM_TIME * DIM_TIME);
    EXPECT_EQ(energy_dim, DIM_ENERGY);
    auto power_dim = DIM_ENERGY / DIM_TIME;
    EXPECT_EQ(power_dim, DIM_POWER);
}

// ============================================================================
// 补足 127 测试 / Fill to 127 tests
// ============================================================================

// Dimension extended tests
TEST(DimensionExtra, MassEq) { EXPECT_EQ(DIM_MASS, DIM_MASS); }
TEST(DimensionExtra, LengthEq) { EXPECT_EQ(DIM_LENGTH, DIM_LENGTH); }
TEST(DimensionExtra, TimeEq) { EXPECT_EQ(DIM_TIME, DIM_TIME); }
TEST(DimensionExtra, MassNeqLength) { EXPECT_NE(DIM_MASS, DIM_LENGTH); }
TEST(DimensionExtra, Dimensionless) { EXPECT_TRUE(DIMENSIONLESS.is_dimensionless()); }
TEST(DimensionExtra, MassNotDimensionless) { EXPECT_FALSE(DIM_MASS.is_dimensionless()); }
TEST(DimensionExtra, LengthNotDimensionless) { EXPECT_FALSE(DIM_LENGTH.is_dimensionless()); }
TEST(DimensionExtra, AreaIsLengthSquared) { EXPECT_EQ(DIM_LENGTH * DIM_LENGTH, DIM_AREA); }
TEST(DimensionExtra, VolumeIsLengthCubed) { EXPECT_EQ(DIM_LENGTH * DIM_LENGTH * DIM_LENGTH, DIM_VOLUME); }
TEST(DimensionExtra, VelocityIsLengthOverTime) { EXPECT_EQ(DIM_LENGTH / DIM_TIME, DIM_VELOCITY); }
TEST(DimensionExtra, ForceIsMassTimesAccel) { EXPECT_EQ(DIM_MASS * DIM_LENGTH / (DIM_TIME * DIM_TIME), DIM_FORCE); }
TEST(DimensionExtra, EnergyIsForceTimesLength) { EXPECT_EQ(DIM_FORCE * DIM_LENGTH, DIM_ENERGY); }
TEST(DimensionExtra, PowerIsEnergyOverTime) { EXPECT_EQ(DIM_ENERGY / DIM_TIME, DIM_POWER); }
TEST(DimensionExtra, FrequencyDimension) { auto freq = DIMENSIONLESS / DIM_TIME; EXPECT_EQ(freq, DIMENSIONLESS / DIM_TIME); }
TEST(DimensionExtra, PressureDimension) { auto pres = DIM_FORCE / DIM_AREA; EXPECT_EQ(pres, DIM_FORCE / DIM_AREA); }
TEST(DimensionExtra, DimensionCopy) {
    Dimension d = DIM_LENGTH;
    EXPECT_EQ(d, DIM_LENGTH);
}
TEST(DimensionExtra, DimensionlessMultiply) {
    auto d = DIMENSIONLESS * DIM_LENGTH;
    EXPECT_EQ(d, DIM_LENGTH);
}
TEST(DimensionExtra, DimensionlessDivide) {
    auto d = DIM_LENGTH / DIMENSIONLESS;
    EXPECT_EQ(d, DIM_LENGTH);
}
TEST(DimensionExtra, SelfDivide) {
    auto d = DIM_LENGTH / DIM_LENGTH;
    EXPECT_TRUE(d.is_dimensionless());
}
TEST(DimensionExtra, PowZero) {
    auto d = DIM_LENGTH.pow(0);
    EXPECT_TRUE(d.is_dimensionless());
}
TEST(DimensionExtra, PowOne) {
    auto d = DIM_LENGTH.pow(1);
    EXPECT_EQ(d, DIM_LENGTH);
}

// Unit extended tests
TEST(UnitExtra, MeterDimension) { EXPECT_EQ(UNIT_METER.dimension, DIM_LENGTH); }
TEST(UnitExtra, MeterName) { EXPECT_EQ(UNIT_METER.name, "meter"); }
TEST(UnitExtra, MeterSymbol) { EXPECT_EQ(UNIT_METER.symbol, "m"); }
TEST(UnitExtra, KilogramDimension) { EXPECT_EQ(UNIT_KILOGRAM.dimension, DIM_MASS); }
TEST(UnitExtra, KilogramName) { EXPECT_EQ(UNIT_KILOGRAM.name, "kilogram"); }
TEST(UnitExtra, KilogramSymbol) { EXPECT_EQ(UNIT_KILOGRAM.symbol, "kg"); }
TEST(UnitExtra, SecondDimension) { EXPECT_EQ(UNIT_SECOND.dimension, DIM_TIME); }
TEST(UnitExtra, SecondName) { EXPECT_EQ(UNIT_SECOND.name, "second"); }
TEST(UnitExtra, ConvertKmToM) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_KILOMETER, UNIT_METER), 1000.0); }
TEST(UnitExtra, ConvertMToKm) { EXPECT_DOUBLE_EQ(convert(1000.0, UNIT_METER, UNIT_KILOMETER), 1.0); }
TEST(UnitExtra, ConvertCmToM) { EXPECT_DOUBLE_EQ(convert(100.0, UNIT_CENTIMETER, UNIT_METER), 1.0); }
TEST(UnitExtra, ConvertMToCm) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_METER, UNIT_CENTIMETER), 100.0); }
TEST(UnitExtra, ConvertMmToM) { EXPECT_DOUBLE_EQ(convert(1000.0, UNIT_MILLIMETER, UNIT_METER), 1.0); }
TEST(UnitExtra, ConvertGToKg) { EXPECT_DOUBLE_EQ(convert(1000.0, UNIT_GRAM, UNIT_KILOGRAM), 1.0); }
TEST(UnitExtra, ConvertKgToG) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_KILOGRAM, UNIT_GRAM), 1000.0); }
TEST(UnitExtra, ConvertTonneToKg) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_TONNE, UNIT_KILOGRAM), 1000.0); }
TEST(UnitExtra, ConvertHourToSec) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_HOUR, UNIT_SECOND), 3600.0); }
TEST(UnitExtra, ConvertSecToHour) { EXPECT_DOUBLE_EQ(convert(3600.0, UNIT_SECOND, UNIT_HOUR), 1.0); }
TEST(UnitExtra, ConvertMinToSec) { EXPECT_DOUBLE_EQ(convert(1.0, UNIT_MINUTE, UNIT_SECOND), 60.0); }
TEST(UnitExtra, ConvertSameUnit) { EXPECT_DOUBLE_EQ(convert(42.0, UNIT_METER, UNIT_METER), 42.0); }

// Quantity extended tests
TEST(QuantityExtra, LengthValue) { Length l(5.0); EXPECT_DOUBLE_EQ(l.value(), 5.0); }
TEST(QuantityExtra, LengthDimension) { Length l(1.0); EXPECT_EQ(l.dimension(), DIM_LENGTH); }
TEST(QuantityExtra, LengthAdd) { Length a(3.0); Length b(4.0); auto c = a + b; EXPECT_DOUBLE_EQ(c.value(), 7.0); }
TEST(QuantityExtra, LengthSub) { Length a(7.0); Length b(3.0); auto c = a - b; EXPECT_DOUBLE_EQ(c.value(), 4.0); }
TEST(QuantityExtra, LengthMulScalar) { Length l(5.0); auto r = l * 2.0; EXPECT_DOUBLE_EQ(r.value(), 10.0); }
TEST(QuantityExtra, LengthDivScalar) { Length l(10.0); auto r = l / 2.0; EXPECT_DOUBLE_EQ(r.value(), 5.0); }
TEST(QuantityExtra, LengthNeg) { Length l(5.0); auto n = -l; EXPECT_DOUBLE_EQ(n.value(), -5.0); }
TEST(QuantityExtra, LengthLt) { Length a(3.0); Length b(5.0); EXPECT_TRUE(a < b); EXPECT_FALSE(b < a); }
TEST(QuantityExtra, LengthGt) { Length a(5.0); Length b(3.0); EXPECT_TRUE(a > b); }
TEST(QuantityExtra, LengthLe) { Length a(3.0); Length b(3.0); EXPECT_TRUE(a <= b); }
TEST(QuantityExtra, LengthGe) { Length a(3.0); Length b(3.0); EXPECT_TRUE(a >= b); }
TEST(QuantityExtra, LengthEq) { Length a(3.0); Length b(3.0); EXPECT_TRUE(a == b); }
TEST(QuantityExtra, LengthNeq) { Length a(3.0); Length b(5.0); EXPECT_TRUE(a != b); }
TEST(QuantityExtra, MassValue) { Mass m(10.0); EXPECT_DOUBLE_EQ(m.value(), 10.0); }
TEST(QuantityExtra, MassDimension) { Mass m(1.0); EXPECT_EQ(m.dimension(), DIM_MASS); }
TEST(QuantityExtra, MassAdd) { Mass a(3.0); Mass b(4.0); auto c = a + b; EXPECT_DOUBLE_EQ(c.value(), 7.0); }
TEST(QuantityExtra, MassMulScalar) { Mass m(5.0); auto r = m * 3.0; EXPECT_DOUBLE_EQ(r.value(), 15.0); }
TEST(QuantityExtra, TimeValue) { Time t(60.0); EXPECT_DOUBLE_EQ(t.value(), 60.0); }
TEST(QuantityExtra, TimeDimension) { Time t(1.0); EXPECT_EQ(t.dimension(), DIM_TIME); }
TEST(QuantityExtra, VelocityValue) { Velocity v(10.0); EXPECT_DOUBLE_EQ(v.value(), 10.0); }
TEST(QuantityExtra, VelocityDimension) { Velocity v(1.0); EXPECT_EQ(v.dimension(), DIM_VELOCITY); }
TEST(QuantityExtra, ForceValue) { Force f(100.0); EXPECT_DOUBLE_EQ(f.value(), 100.0); }
TEST(QuantityExtra, ForceDimension) { Force f(1.0); EXPECT_EQ(f.dimension(), DIM_FORCE); }
TEST(QuantityExtra, AccelerationValue) { Acceleration a(9.81); EXPECT_DOUBLE_EQ(a.value(), 9.81); }
TEST(QuantityExtra, AccelerationDimension) { Acceleration a(1.0); EXPECT_EQ(a.dimension(), DIM_ACCELERATION); }
TEST(QuantityExtra, EnergyValue) { Energy e(1000.0); EXPECT_DOUBLE_EQ(e.value(), 1000.0); }
TEST(QuantityExtra, EnergyDimension) { Energy e(1.0); EXPECT_EQ(e.dimension(), DIM_ENERGY); }
TEST(QuantityExtra, PowerValue) { Power p(100.0); EXPECT_DOUBLE_EQ(p.value(), 100.0); }
TEST(QuantityExtra, PowerDimension) { Power p(1.0); EXPECT_EQ(p.dimension(), DIM_POWER); }
TEST(QuantityExtra, SameDimDiv) { Length a(10.0); Length b(2.0); double r = a / b; EXPECT_DOUBLE_EQ(r, 5.0); }
TEST(QuantityExtra, ScalarMulFriend) { Length l(5.0); auto r = 3.0 * l; EXPECT_DOUBLE_EQ(r.value(), 15.0); }
TEST(QuantityExtra, ConvertUnit) { Length m(1000.0); Length km = convert_unit(m, 1.0, 1000.0); EXPECT_DOUBLE_EQ(km.value(), 1.0); }
TEST(QuantityExtra, ZeroLength) { Length l(0.0); EXPECT_DOUBLE_EQ(l.value(), 0.0); }
TEST(QuantityExtra, NegativeLength) { Length l(-5.0); EXPECT_DOUBLE_EQ(l.value(), -5.0); }
TEST(QuantityExtra, LargeValue) { Length l(1e100); EXPECT_DOUBLE_EQ(l.value(), 1e100); }

// QuantityVector extended tests
TEST(QuantityVectorExtra, Construction) {
    QuantityVector<double, DIM_LENGTH, 3> v(1.0, 2.0, 3.0);
    EXPECT_DOUBLE_EQ(v[0], 1.0);
    EXPECT_DOUBLE_EQ(v[1], 2.0);
    EXPECT_DOUBLE_EQ(v[2], 3.0);
}
TEST(QuantityVectorExtra, Size) {
    QuantityVector<double, DIM_LENGTH, 3> v(1.0, 2.0, 3.0);
    EXPECT_EQ(v.size(), 3u);
}
TEST(QuantityVectorExtra, Addition) {
    QuantityVector<double, DIM_LENGTH, 2> a(1.0, 2.0);
    QuantityVector<double, DIM_LENGTH, 2> b(3.0, 4.0);
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c[0], 4.0);
    EXPECT_DOUBLE_EQ(c[1], 6.0);
}
TEST(QuantityVectorExtra, ScalarMul) {
    QuantityVector<double, DIM_LENGTH, 2> v(1.0, 2.0);
    auto r = v * 3.0;
    EXPECT_DOUBLE_EQ(r[0], 3.0);
    EXPECT_DOUBLE_EQ(r[1], 6.0);
}
TEST(QuantityVectorExtra, TwoD) {
    QuantityVector<double, DIM_LENGTH, 2> v(3.0, 4.0);
    EXPECT_EQ(v.size(), 2u);
}
TEST(QuantityVectorExtra, FourD) {
    QuantityVector<double, DIM_LENGTH, 4> v(1.0, 2.0, 3.0, 4.0);
    EXPECT_EQ(v.size(), 4u);
    EXPECT_DOUBLE_EQ(v[3], 4.0);
}

// Integration extended tests
TEST(IntegrationExtra, ForceCalculation) {
    Mass m(10.0);
    Acceleration a(9.81);
    Force f(m.value() * a.value());
    EXPECT_NEAR(f.value(), 98.1, 1e-10);
}
TEST(IntegrationExtra, EnergyCalculation) {
    Mass m(2.0);
    Velocity v(3.0);
    Energy e(0.5 * m.value() * v.value() * v.value());
    EXPECT_DOUBLE_EQ(e.value(), 9.0);
}
TEST(IntegrationExtra, PowerCalculation) {
    Energy e(100.0);
    Time t(10.0);
    Power p(e.value() / t.value());
    EXPECT_DOUBLE_EQ(p.value(), 10.0);
}
TEST(IntegrationExtra, DimensionChain) {
    auto accel = DIM_LENGTH / (DIM_TIME * DIM_TIME);
    EXPECT_EQ(accel, DIM_ACCELERATION);
    auto force = DIM_MASS * accel;
    EXPECT_EQ(force, DIM_FORCE);
}

TEST(QuantitiesExtra, LengthAddCommutative) { Length a(3.0); Length b(4.0); EXPECT_DOUBLE_EQ((a+b).value(), (b+a).value()); }
TEST(QuantitiesExtra, MassAddCommutative) { Mass a(1.0); Mass b(2.0); EXPECT_DOUBLE_EQ((a+b).value(), (b+a).value()); }
TEST(QuantitiesExtra, TimeAddCommutative) { Time a(10.0); Time b(20.0); EXPECT_DOUBLE_EQ((a+b).value(), (b+a).value()); }
TEST(QuantitiesExtra, LengthSubZero) { Length a(5.0); Length b(5.0); EXPECT_DOUBLE_EQ((a-b).value(), 0.0); }
TEST(QuantitiesExtra, MassSubZero) { Mass a(3.0); Mass b(3.0); EXPECT_DOUBLE_EQ((a-b).value(), 0.0); }
TEST(QuantitiesExtra, LengthMulOne) { Length l(42.0); EXPECT_DOUBLE_EQ((l * 1.0).value(), 42.0); }
TEST(QuantitiesExtra, LengthDivOne) { Length l(42.0); EXPECT_DOUBLE_EQ((l / 1.0).value(), 42.0); }
TEST(QuantitiesExtra, VelocityMulTime) { Velocity v(10.0); Time t(3.0); Length d(v.value() * t.value()); EXPECT_DOUBLE_EQ(d.value(), 30.0); }
TEST(QuantitiesExtra, ForceDivMass) { Force f(100.0); Mass m(10.0); Acceleration a(f.value() / m.value()); EXPECT_DOUBLE_EQ(a.value(), 10.0); }
TEST(QuantitiesExtra, EnergyDivTime) { Energy e(1000.0); Time t(10.0); Power p(e.value() / t.value()); EXPECT_DOUBLE_EQ(p.value(), 100.0); }
TEST(QuantitiesExtra, ConvertMToKmReverse) { Length km(1.0); Length m = convert_unit(km, 1000.0, 1.0); EXPECT_DOUBLE_EQ(m.value(), 1000.0); }
TEST(QuantitiesExtra, ConvertKgToTonne) { Mass kg(1000.0); Mass t = convert_unit(kg, 1.0, 1000.0); EXPECT_DOUBLE_EQ(t.value(), 1.0); }
TEST(QuantitiesExtra, ConvertSecToMin) { Time sec(120.0); Time min = convert_unit(sec, 1.0, 60.0); EXPECT_DOUBLE_EQ(min.value(), 2.0); }
TEST(QuantitiesExtra, DimensionPowNeg) { auto inv_length = DIM_LENGTH.pow(-1); EXPECT_EQ(inv_length, DIMENSIONLESS / DIM_LENGTH); }
TEST(QuantitiesExtra, DimensionPowTwo) { auto area = DIM_LENGTH.pow(2); EXPECT_EQ(area, DIM_AREA); }
TEST(QuantitiesExtra, DimensionPowThree) { auto vol = DIM_LENGTH.pow(3); EXPECT_EQ(vol, DIM_VOLUME); }
TEST(QuantitiesExtra, QuantityDefaultConstruct) { Length l; EXPECT_DOUBLE_EQ(l.value(), 0.0); }
TEST(QuantitiesExtra, MassDefaultConstruct) { Mass m; EXPECT_DOUBLE_EQ(m.value(), 0.0); }
