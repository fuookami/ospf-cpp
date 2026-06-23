/// ospf_math geometry/ordinary/operator 测试
/// 覆盖点、线段、三角形、圆、四边形、包围盒、普通数学函数、运算符工具。

#include <gtest/gtest.h>

#include <ospf/math/geometry/point.hpp>
#include <ospf/math/geometry/line.hpp>
#include <ospf/math/geometry/triangle.hpp>
#include <ospf/math/geometry/circle.hpp>
#include <ospf/math/geometry/quadrilateral.hpp>
#include <ospf/math/geometry/bbox.hpp>
#include <ospf/math/ordinary.hpp>
#include <ospf/math/operator.hpp>

#include <cmath>
#include <numbers>

using namespace ospf::math::geometry;
using namespace ospf::math;

// ============================================================================
// Point 测试 / Point tests
// ============================================================================

TEST(Point, Construction) {
    Point2d p = {3.0, 4.0};
    EXPECT_DOUBLE_EQ(p[0], 3.0);
    EXPECT_DOUBLE_EQ(p[1], 4.0);
    EXPECT_EQ(p.dimension(), 2u);
}

TEST(Point, Distance) {
    Point2d a = {0.0, 0.0};
    Point2d b = {3.0, 4.0};
    EXPECT_DOUBLE_EQ(a.distance_to(b), 5.0);
    EXPECT_DOUBLE_EQ(a.distance_squared_to(b), 25.0);
}

TEST(Point, Midpoint) {
    Point2d a = {0.0, 0.0};
    Point2d b = {4.0, 6.0};
    auto mid = a.midpoint(b);
    EXPECT_DOUBLE_EQ(mid[0], 2.0);
    EXPECT_DOUBLE_EQ(mid[1], 3.0);
}

TEST(Point, Equality) {
    Point2d a = {1.0, 2.0};
    Point2d b = {1.0, 2.0};
    Point2d c = {1.0, 3.0};
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(Point, ThreeDimensional) {
    Point3d p = {1.0, 2.0, 3.0};
    EXPECT_EQ(p.dimension(), 3u);
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 2.0);
    EXPECT_DOUBLE_EQ(p[2], 3.0);
}

// ============================================================================
// Segment 测试 / Segment tests
// ============================================================================

TEST(Segment, Basic) {
    Segment2d seg(Point2d({0.0, 0.0}), Point2d({3.0, 4.0}));
    EXPECT_DOUBLE_EQ(seg.length(), 5.0);

    auto mid = seg.midpoint();
    EXPECT_DOUBLE_EQ(mid[0], 1.5);
    EXPECT_DOUBLE_EQ(mid[1], 2.0);
}

// ============================================================================
// Triangle 测试 / Triangle tests
// ============================================================================

TEST(Triangle, Basic) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 4.0}));

    EXPECT_DOUBLE_EQ(tri.ab(), 3.0);
    EXPECT_DOUBLE_EQ(tri.bc(), 5.0);
    EXPECT_DOUBLE_EQ(tri.ca(), 4.0);
    EXPECT_DOUBLE_EQ(tri.perimeter(), 12.0);
}

TEST(Triangle, Area) {
    // 直角三角形：底 3，高 4，面积 6
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 4.0}));
    EXPECT_NEAR(tri.area(), 6.0, 1e-10);
}

TEST(Triangle, Centroid) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 3.0}));
    auto c = tri.centroid();
    EXPECT_NEAR(c[0], 1.0, 1e-10);
    EXPECT_NEAR(c[1], 1.0, 1e-10);
}

// ============================================================================
// Circle 测试 / Circle tests
// ============================================================================

TEST(Circle, Basic) {
    Circle2d circle(Point2d({0.0, 0.0}), 5.0);
    EXPECT_DOUBLE_EQ(circle.radius(), 5.0);
    EXPECT_NEAR(circle.circumference(), 2.0 * std::numbers::pi * 5.0, 1e-10);
    EXPECT_NEAR(circle.area(), std::numbers::pi * 25.0, 1e-10);
}

TEST(Circle, Contains) {
    Circle2d circle(Point2d({0.0, 0.0}), 5.0);
    EXPECT_TRUE(circle.contains(Point2d({0.0, 0.0})));
    EXPECT_TRUE(circle.contains(Point2d({3.0, 4.0})));
    EXPECT_FALSE(circle.contains(Point2d({10.0, 0.0})));
}

// ============================================================================
// Quadrilateral 测试 / Quadrilateral tests
// ============================================================================

TEST(Quadrilateral, Square) {
    Quadrilateral2d quad(
        Point2d({0.0, 0.0}), Point2d({1.0, 0.0}),
        Point2d({1.0, 1.0}), Point2d({0.0, 1.0})
    );
    EXPECT_DOUBLE_EQ(quad.perimeter(), 4.0);
    EXPECT_NEAR(quad.area(), 1.0, 1e-10);
}

// ============================================================================
// BoundingBox 测试 / Bounding box tests
// ============================================================================

TEST(BoundingBox, Basic) {
    auto bbox = BoundingBox2d::from_points(Point2d({1.0, 2.0}), Point2d({5.0, 8.0}));
    EXPECT_DOUBLE_EQ(bbox.min()[0], 1.0);
    EXPECT_DOUBLE_EQ(bbox.min()[1], 2.0);
    EXPECT_DOUBLE_EQ(bbox.max()[0], 5.0);
    EXPECT_DOUBLE_EQ(bbox.max()[1], 8.0);

    auto center = bbox.center();
    EXPECT_DOUBLE_EQ(center[0], 3.0);
    EXPECT_DOUBLE_EQ(center[1], 5.0);
}

TEST(BoundingBox, Contains) {
    auto bbox = BoundingBox2d::from_points(Point2d({0.0, 0.0}), Point2d({10.0, 10.0}));
    EXPECT_TRUE(bbox.contains(Point2d({5.0, 5.0})));
    EXPECT_TRUE(bbox.contains(Point2d({0.0, 0.0})));
    EXPECT_FALSE(bbox.contains(Point2d({11.0, 5.0})));
}

// ============================================================================
// Ordinary math 测试 / Ordinary math tests
// ============================================================================

TEST(Ordinary, Abs) {
    EXPECT_DOUBLE_EQ(abs(-5.0), 5.0);
    EXPECT_DOUBLE_EQ(abs(5.0), 5.0);
    EXPECT_EQ(abs(-5), 5);
}

TEST(Ordinary, Reciprocal) {
    EXPECT_DOUBLE_EQ(reciprocal(2.0), 0.5);
    EXPECT_DOUBLE_EQ(reciprocal(4.0), 0.25);
}

TEST(Ordinary, SquareCube) {
    EXPECT_DOUBLE_EQ(square(3.0), 9.0);
    EXPECT_DOUBLE_EQ(cube(3.0), 27.0);
}

TEST(Ordinary, TrigDegrees) {
    EXPECT_NEAR(sin_deg(30.0), 0.5, 1e-10);
    EXPECT_NEAR(cos_deg(60.0), 0.5, 1e-10);
    EXPECT_NEAR(tan_deg(45.0), 1.0, 1e-10);
}

TEST(Ordinary, Logarithms) {
    EXPECT_NEAR(ln(std::numbers::e), 1.0, 1e-10);
    EXPECT_NEAR(log10(100.0), 2.0, 1e-10);
    EXPECT_NEAR(log2(8.0), 3.0, 1e-10);
}

TEST(Ordinary, Exponential) {
    EXPECT_NEAR(exp(0.0), 1.0, 1e-10);
    EXPECT_NEAR(exp(1.0), std::numbers::e, 1e-10);
}

TEST(Ordinary, IsPrime) {
    EXPECT_FALSE(is_prime(0));
    EXPECT_FALSE(is_prime(1));
    EXPECT_TRUE(is_prime(2));
    EXPECT_TRUE(is_prime(3));
    EXPECT_FALSE(is_prime(4));
    EXPECT_TRUE(is_prime(5));
    EXPECT_TRUE(is_prime(7));
    EXPECT_FALSE(is_prime(9));
    EXPECT_TRUE(is_prime(13));
}

// ============================================================================
// Operator 测试 / Operator tests
// ============================================================================

TEST(Operator, ApproxEqual) {
    EXPECT_TRUE(approx_equal(1.0, 1.0));
    EXPECT_TRUE(approx_equal(1.0, 1.0 + 1e-15, 1e-14));
    EXPECT_FALSE(approx_equal(1.0, 2.0));
}

TEST(Operator, ApproxZero) {
    EXPECT_TRUE(approx_zero(0.0));
    EXPECT_TRUE(approx_zero(1e-16));
    EXPECT_FALSE(approx_zero(1.0));
}

TEST(Operator, InRange) {
    EXPECT_TRUE(in_range(5, 0, 10));
    EXPECT_TRUE(in_range(0, 0, 10));
    EXPECT_TRUE(in_range(10, 0, 10));
    EXPECT_FALSE(in_range(11, 0, 10));
}

TEST(Operator, InOpenRange) {
    EXPECT_TRUE(in_open_range(5, 0, 10));
    EXPECT_FALSE(in_open_range(0, 0, 10));
    EXPECT_FALSE(in_open_range(10, 0, 10));
}

TEST(Operator, Lerp) {
    EXPECT_DOUBLE_EQ(lerp(0.0, 10.0, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(lerp(0.0, 10.0, 0.5), 5.0);
    EXPECT_DOUBLE_EQ(lerp(0.0, 10.0, 1.0), 10.0);
}

TEST(Operator, Clamp) {
    EXPECT_EQ(clamp(5, 0, 10), 5);
    EXPECT_EQ(clamp(-1, 0, 10), 0);
    EXPECT_EQ(clamp(15, 0, 10), 10);
}

TEST(Operator, Sign) {
    EXPECT_EQ(sign(5), 1);
    EXPECT_EQ(sign(-5), -1);
    EXPECT_EQ(sign(0), 0);
}

// ============================================================================
// 综合测试 / Integration tests
// ============================================================================

TEST(Integration, GeometryWorkflow) {
    // 创建三角形
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({4.0, 0.0}), Point2d({0.0, 3.0}));

    // 验证面积
    EXPECT_NEAR(tri.area(), 6.0, 1e-10);

    // 创建外接圆（简单版本：以重心为圆心）
    auto centroid = tri.centroid();
    Circle2d circle(centroid, 2.5);

    // 验证重心在圆内
    EXPECT_TRUE(circle.contains(centroid));
}
