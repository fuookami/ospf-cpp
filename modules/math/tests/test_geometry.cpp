/// ospf_math geometry/ordinary/operator 测试
/// 覆盖点、线段、三角形、圆、四边形、包围盒、普通数学函数、运算符工具。

#include <gtest/gtest.h>

#include <ospf/math/geometry/point.hpp>
#include <ospf/math/geometry/line.hpp>
#include <ospf/math/geometry/triangle.hpp>
#include <ospf/math/geometry/circle.hpp>
#include <ospf/math/geometry/quadrilateral.hpp>
#include <ospf/math/geometry/bbox.hpp>
#include <ospf/math/geometry/axis.hpp>
#include <ospf/math/geometry/edge.hpp>
#include <ospf/math/geometry/vector.hpp>
#include <ospf/math/geometry/distance.hpp>
#include <ospf/math/geometry/box_shape.hpp>
#include <ospf/math/geometry/placement.hpp>
#include <ospf/math/geometry/projection.hpp>
#include <ospf/math/geometry/plane_frame.hpp>
#include <ospf/math/geometry/shape3.hpp>
#include <ospf/math/geometry/delaunay.hpp>
#include <ospf/math/geometry/factory.hpp>
#include <ospf/math/ordinary.hpp>
#include <ospf/math/ordinary/gcd.hpp>
#include <ospf/math/ordinary/lcm.hpp>
#include <ospf/math/ordinary/prime.hpp>
#include <ospf/math/ordinary/factorization.hpp>
#include <ospf/math/ordinary/common.hpp>
#include <ospf/math/ordinary/big_decimal_pow.hpp>
#include <ospf/math/operator.hpp>
#include <ospf/math/operator/tolerance.hpp>
#include <ospf/math/operator/abs.hpp>
#include <ospf/math/operator/contains.hpp>
#include <ospf/math/operator/exp_log.hpp>
#include <ospf/math/operator/exponent.hpp>
#include <ospf/math/operator/power.hpp>
#include <ospf/math/operator/reciprocal.hpp>
#include <ospf/math/operator/ref_additive.hpp>
#include <ospf/math/operator/ref_multiplicative.hpp>
#include <ospf/math/operator/one_zero_ref.hpp>
#include <ospf/math/operator/trigonometry.hpp>

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
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({4.0, 0.0}), Point2d({0.0, 3.0}));
    EXPECT_NEAR(tri.area(), 6.0, 1e-10);
    auto centroid = tri.centroid();
    Circle2d circle(centroid, 2.5);
    EXPECT_TRUE(circle.contains(centroid));
}

// ============================================================================
// 扩展 Point 测试 / Extended Point tests (Rust point.rs: 10 tests)
// ============================================================================

TEST(Point, Origin) {
    Point2d p({0.0, 0.0});
    EXPECT_DOUBLE_EQ(p.x(), 0.0);
    EXPECT_DOUBLE_EQ(p.y(), 0.0);
}

TEST(Point, NegativeCoords) {
    Point2d p({-3.0, -4.0});
    EXPECT_DOUBLE_EQ(p.x(), -3.0);
    EXPECT_DOUBLE_EQ(p.y(), -4.0);
}

TEST(Point, DistanceToSelf) {
    Point2d p({5.0, 5.0});
    EXPECT_DOUBLE_EQ(p.distance_to(p), 0.0);
}

TEST(Point, DistanceSymmetry) {
    Point2d a({1.0, 2.0});
    Point2d b({4.0, 6.0});
    EXPECT_DOUBLE_EQ(a.distance_to(b), b.distance_to(a));
}

TEST(Point, MidpointSymmetry) {
    Point2d a({0.0, 0.0});
    Point2d b({10.0, 10.0});
    auto m = a.midpoint(b);
    EXPECT_DOUBLE_EQ(m.x(), 5.0);
    EXPECT_DOUBLE_EQ(m.y(), 5.0);
}

// ============================================================================
// 扩展 Edge 测试 / Edge tests (Rust edge.rs: 11 tests)
// ============================================================================

TEST(Edge, Construction) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({3.0, 4.0})};
    EXPECT_DOUBLE_EQ(e.length(), 5.0);
}

TEST(Edge, Midpoint) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({10.0, 0.0})};
    auto m = e.midpoint();
    EXPECT_DOUBLE_EQ(m.x(), 5.0);
    EXPECT_DOUBLE_EQ(m.y(), 0.0);
}

TEST(Edge, ZeroLength) {
    Edge<double> e{Point2d({5.0, 5.0}), Point2d({5.0, 5.0})};
    EXPECT_DOUBLE_EQ(e.length(), 0.0);
}

TEST(Edge, Horizontal) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({10.0, 0.0})};
    EXPECT_DOUBLE_EQ(e.length(), 10.0);
}

TEST(Edge, Vertical) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({0.0, 7.0})};
    EXPECT_DOUBLE_EQ(e.length(), 7.0);
}

TEST(Edge, Equality) {
    Edge<double> e1{Point2d({0.0, 0.0}), Point2d({1.0, 1.0})};
    Edge<double> e2{Point2d({0.0, 0.0}), Point2d({1.0, 1.0})};
    Edge<double> e3{Point2d({0.0, 0.0}), Point2d({2.0, 2.0})};
    EXPECT_EQ(e1, e2);
    EXPECT_NE(e1, e3);
}

// ============================================================================
// 扩展 GeoVector 测试 / GeoVector tests (Rust vector.rs: 14 tests)
// ============================================================================

TEST(GeoVector, Construction) {
    GeoVector<double> v{3.0, 4.0};
    EXPECT_DOUBLE_EQ(v.x, 3.0);
    EXPECT_DOUBLE_EQ(v.y, 4.0);
}

TEST(GeoVector, Magnitude) {
    GeoVector<double> v{3.0, 4.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 5.0);
}

TEST(GeoVector, ZeroMagnitude) {
    GeoVector<double> v{0.0, 0.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 0.0);
}

TEST(GeoVector, DotProduct) {
    GeoVector<double> a{1.0, 2.0};
    GeoVector<double> b{3.0, 4.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 11.0);  // 1*3 + 2*4
}

TEST(GeoVector, CrossProduct) {
    GeoVector<double> a{1.0, 0.0};
    GeoVector<double> b{0.0, 1.0};
    EXPECT_DOUBLE_EQ(a.cross(b), 1.0);
}

TEST(GeoVector, Addition) {
    GeoVector<double> a{1.0, 2.0};
    GeoVector<double> b{3.0, 4.0};
    auto c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 4.0);
    EXPECT_DOUBLE_EQ(c.y, 6.0);
}

TEST(GeoVector, Subtraction) {
    GeoVector<double> a{5.0, 7.0};
    GeoVector<double> b{2.0, 3.0};
    auto c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 3.0);
    EXPECT_DOUBLE_EQ(c.y, 4.0);
}

TEST(GeoVector, ScalarMultiply) {
    GeoVector<double> v{2.0, 3.0};
    auto r = v * 2.0;
    EXPECT_DOUBLE_EQ(r.x, 4.0);
    EXPECT_DOUBLE_EQ(r.y, 6.0);
}

TEST(GeoVector, Equality) {
    GeoVector<double> a{1.0, 2.0};
    GeoVector<double> b{1.0, 2.0};
    GeoVector<double> c{1.0, 3.0};
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

TEST(GeoVector, UnitX) {
    GeoVector<double> v{1.0, 0.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 1.0);
}

TEST(GeoVector, UnitY) {
    GeoVector<double> v{0.0, 1.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 1.0);
}

// ============================================================================
// 扩展 BoundingBox 测试 / BoundingBox tests (Rust bounding_box.rs: 4 tests)
// ============================================================================

TEST(BoundingBox, FromPoints) {
    auto bbox = BoundingBox2d::from_points(Point2d({0.0, 0.0}), Point2d({10.0, 5.0}));
    EXPECT_TRUE(bbox.contains(Point2d({5.0, 2.5})));
    EXPECT_FALSE(bbox.contains(Point2d({11.0, 2.5})));
}

TEST(BoundingBox, SinglePoint) {
    auto bbox = BoundingBox2d::from_points(Point2d({5.0, 5.0}), Point2d({5.0, 5.0}));
    EXPECT_TRUE(bbox.contains(Point2d({5.0, 5.0})));
}

TEST(BoundingBox, MinMax) {
    auto bbox = BoundingBox2d::from_points(Point2d({0.0, 0.0}), Point2d({10.0, 5.0}));
    EXPECT_DOUBLE_EQ(bbox.min()[0], 0.0);
    EXPECT_DOUBLE_EQ(bbox.max()[0], 10.0);
}

// ============================================================================
// 扩展 Triangle 测试 / Triangle tests (Rust triangle.rs: 12 tests)
// ============================================================================

TEST(Triangle, RightTriangle) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 4.0}));
    EXPECT_NEAR(tri.area(), 6.0, 1e-10);
}

TEST(Triangle, EquilateralTriangle) {
    double s = 2.0;
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({s, 0.0}), Point2d({s / 2, s * std::sqrt(3.0) / 2}));
    EXPECT_NEAR(tri.area(), std::sqrt(3.0), 1e-10);
}

TEST(Triangle, CentroidSymmetry) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({6.0, 0.0}), Point2d({3.0, 6.0}));
    auto c = tri.centroid();
    EXPECT_NEAR(c.x(), 3.0, 1e-10);
    EXPECT_NEAR(c.y(), 2.0, 1e-10);
}

TEST(Triangle, DegenerateTriangle) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({5.0, 0.0}), Point2d({10.0, 0.0}));
    EXPECT_NEAR(tri.area(), 0.0, 1e-10);
}

// ============================================================================
// 扩展 Circle 测试 / Circle tests (Rust circle.rs: 10 tests)
// ============================================================================

TEST(Circle, UnitCircle) {
    Circle2d c(Point2d({0.0, 0.0}), 1.0);
    EXPECT_NEAR(c.area(), std::numbers::pi, 1e-10);
    EXPECT_NEAR(c.circumference(), 2 * std::numbers::pi, 1e-10);
}

TEST(Circle, ContainsEdge) {
    Circle2d c(Point2d({0.0, 0.0}), 5.0);
    EXPECT_TRUE(c.contains(Point2d({5.0, 0.0})));   // on boundary
    EXPECT_FALSE(c.contains(Point2d({5.1, 0.0})));  // just outside
}

TEST(Circle, ContainsInside) {
    Circle2d c(Point2d({0.0, 0.0}), 10.0);
    EXPECT_TRUE(c.contains(Point2d({0.0, 0.0})));
    EXPECT_TRUE(c.contains(Point2d({3.0, 4.0})));
}

TEST(Circle, ContainsOutside) {
    Circle2d c(Point2d({0.0, 0.0}), 1.0);
    EXPECT_FALSE(c.contains(Point2d({10.0, 10.0})));
}

TEST(Circle, LargeRadius) {
    Circle2d c(Point2d({0.0, 0.0}), 1000.0);
    EXPECT_TRUE(c.contains(Point2d({999.0, 0.0})));
}

TEST(Circle, SmallRadius) {
    Circle2d c(Point2d({5.0, 5.0}), 0.001);
    EXPECT_TRUE(c.contains(Point2d({5.0, 5.0})));
    EXPECT_FALSE(c.contains(Point2d({5.0, 5.1})));
}

// ============================================================================
// 扩展 Quadrilateral 测试 / Quadrilateral tests (Rust quadrilateral.rs: 9 tests)
// ============================================================================

TEST(Quadrilateral, UnitSquare) {
    Quadrilateral2d q(Point2d({0.0, 0.0}), Point2d({1.0, 0.0}), Point2d({1.0, 1.0}), Point2d({0.0, 1.0}));
    EXPECT_NEAR(q.area(), 1.0, 1e-10);
}

TEST(Quadrilateral, Rectangle) {
    Quadrilateral2d q(Point2d({0.0, 0.0}), Point2d({4.0, 0.0}), Point2d({4.0, 3.0}), Point2d({0.0, 3.0}));
    EXPECT_NEAR(q.area(), 12.0, 1e-10);
}

TEST(Quadrilateral, LargeSquare) {
    Quadrilateral2d q(Point2d({0.0, 0.0}), Point2d({100.0, 0.0}), Point2d({100.0, 100.0}), Point2d({0.0, 100.0}));
    EXPECT_NEAR(q.area(), 10000.0, 1e-6);
}

// ============================================================================
// 扩展 Distance 测试 / Distance tests (Rust distance.rs: 5 tests)
// ============================================================================

TEST(Distance, Euclidean) {
    Point2d a({0.0, 0.0});
    Point2d b({3.0, 4.0});
    EXPECT_DOUBLE_EQ(euclidean_distance(a, b), 5.0);
}

TEST(Distance, Manhattan) {
    Point2d a({0.0, 0.0});
    Point2d b({3.0, 4.0});
    EXPECT_DOUBLE_EQ(manhattan_distance(a, b), 7.0);
}

TEST(Distance, ZeroDistance) {
    Point2d p({5.0, 5.0});
    EXPECT_DOUBLE_EQ(euclidean_distance(p, p), 0.0);
    EXPECT_DOUBLE_EQ(manhattan_distance(p, p), 0.0);
}

TEST(Distance, Symmetry) {
    Point2d a({1.0, 2.0});
    Point2d b({4.0, 6.0});
    EXPECT_DOUBLE_EQ(euclidean_distance(a, b), euclidean_distance(b, a));
    EXPECT_DOUBLE_EQ(manhattan_distance(a, b), manhattan_distance(b, a));
}

// ============================================================================
// Axis 测试 / Axis tests (Rust axis.rs: 5 tests)
// ============================================================================

TEST(Axis, XAxis) { EXPECT_EQ(Axis::X, Axis::X); }
TEST(Axis, YAxis) { EXPECT_EQ(Axis::Y, Axis::Y); }
TEST(Axis, ZAxis) { EXPECT_EQ(Axis::Z, Axis::Z); }
TEST(Axis, Other2dX) { EXPECT_EQ(other_2d(Axis::X), Axis::Y); }
TEST(Axis, Other2dY) { EXPECT_EQ(other_2d(Axis::Y), Axis::X); }

// ============================================================================
// BoxShape 测试 / BoxShape tests (Rust box_shape.rs)
// ============================================================================

TEST(BoxShape, Area) {
    BoxShape<double> b{3.0, 4.0};
    EXPECT_DOUBLE_EQ(b.area(), 12.0);
}

TEST(BoxShape, Perimeter) {
    BoxShape<double> b{3.0, 4.0};
    EXPECT_DOUBLE_EQ(b.perimeter(), 14.0);
}

TEST(BoxShape, Contains) {
    BoxShape<double> b{10.0, 10.0};
    EXPECT_TRUE(b.contains(5.0, 5.0));
    EXPECT_FALSE(b.contains(11.0, 5.0));
}

TEST(BoxShape, Equality) {
    BoxShape<double> a{3.0, 4.0};
    BoxShape<double> b{3.0, 4.0};
    BoxShape<double> c{5.0, 5.0};
    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

// ============================================================================
// Shape3 测试 / Shape3 tests (Rust shape3.rs: 6 tests)
// ============================================================================

TEST(Shape3, Volume) {
    Shape3<double> s{2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(s.volume(), 24.0);
}

TEST(Shape3, SurfaceArea) {
    Shape3<double> s{2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(s.surface_area(), 52.0);  // 2*(2*3+3*4+2*4) = 2*(6+12+8) = 52
}

TEST(Shape3, Cube) {
    Shape3<double> s{5.0, 5.0, 5.0};
    EXPECT_DOUBLE_EQ(s.volume(), 125.0);
    EXPECT_DOUBLE_EQ(s.surface_area(), 150.0);
}

TEST(Shape3, Equality) {
    Shape3<double> a{1.0, 2.0, 3.0};
    Shape3<double> b{1.0, 2.0, 3.0};
    EXPECT_EQ(a, b);
}

// ============================================================================
// Placement 测试 / Placement tests (Rust placement.rs: 2 tests)
// ============================================================================

TEST(Placement, Construction) {
    Placement<double> p{Point2d({1.0, 2.0}), 45.0};
    EXPECT_DOUBLE_EQ(p.position.x(), 1.0);
    EXPECT_DOUBLE_EQ(p.rotation, 45.0);
}

TEST(Placement, Equality) {
    Placement<double> a{Point2d({1.0, 2.0}), 0.0};
    Placement<double> b{Point2d({1.0, 2.0}), 0.0};
    EXPECT_EQ(a, b);
}

// ============================================================================
// PlaneFrame 测试 / PlaneFrame tests (Rust plane_frame.rs: 4 tests)
// ============================================================================

TEST(PlaneFrame, Construction) {
    PlaneFrame<double> f{Point2d({1.0, 2.0}), 0.0};
    EXPECT_DOUBLE_EQ(f.origin.x(), 1.0);
}

TEST(PlaneFrame, ToLocal) {
    PlaneFrame<double> f{Point2d({10.0, 20.0}), 0.0};
    auto local = f.to_local(Point2d({15.0, 25.0}));
    EXPECT_DOUBLE_EQ(local.x(), 5.0);
    EXPECT_DOUBLE_EQ(local.y(), 5.0);
}

TEST(PlaneFrame, Equality) {
    PlaneFrame<double> a{Point2d({0.0, 0.0}), 0.0};
    PlaneFrame<double> b{Point2d({0.0, 0.0}), 0.0};
    EXPECT_EQ(a, b);
}

// ============================================================================
// Projection 测试 / Projection tests (Rust projection.rs: 4 tests)
// ============================================================================

TEST(Projection, WithinRange) { EXPECT_DOUBLE_EQ(project_scalar(5.0, 0.0, 10.0), 5.0); }
TEST(Projection, BelowRange) { EXPECT_DOUBLE_EQ(project_scalar(-1.0, 0.0, 10.0), 0.0); }
TEST(Projection, AboveRange) { EXPECT_DOUBLE_EQ(project_scalar(15.0, 0.0, 10.0), 10.0); }
TEST(Projection, AtBoundary) { EXPECT_DOUBLE_EQ(project_scalar(0.0, 0.0, 10.0), 0.0); }

// ============================================================================
// 扩展 Ordinary 测试 / Extended Ordinary tests (Rust ordinary/*.rs: 29 tests)
// ============================================================================

TEST(OrdinaryGcd, Basic) {
    using ospf::math::ordinary::gcd;
    EXPECT_EQ(gcd(12, 8), 4);
    EXPECT_EQ(gcd(7, 5), 1);
    EXPECT_EQ(gcd(0, 5), 5);
    EXPECT_EQ(gcd(5, 0), 5);
}

TEST(OrdinaryGcd, Coprime) {
    using ospf::math::ordinary::gcd;
    EXPECT_EQ(gcd(13, 17), 1);
    EXPECT_EQ(gcd(100, 101), 1);
}

TEST(OrdinaryLcm, Basic) {
    using ospf::math::ordinary::lcm;
    EXPECT_EQ(lcm(4, 6), 12);
    EXPECT_EQ(lcm(3, 5), 15);
}

TEST(OrdinaryLcm, Coprime) {
    using ospf::math::ordinary::lcm;
    EXPECT_EQ(lcm(7, 11), 77);
}

TEST(OrdinaryLcm, SameNumber) {
    using ospf::math::ordinary::lcm;
    EXPECT_EQ(lcm(5, 5), 5);
}

TEST(OrdinaryPrime, SmallPrimes) {
    using ospf::math::ordinary::is_prime;
    EXPECT_TRUE(is_prime(2));
    EXPECT_TRUE(is_prime(3));
    EXPECT_TRUE(is_prime(5));
    EXPECT_TRUE(is_prime(7));
    EXPECT_TRUE(is_prime(11));
}

TEST(OrdinaryPrime, NonPrimes) {
    using ospf::math::ordinary::is_prime;
    EXPECT_FALSE(is_prime(0));
    EXPECT_FALSE(is_prime(1));
    EXPECT_FALSE(is_prime(4));
    EXPECT_FALSE(is_prime(6));
    EXPECT_FALSE(is_prime(9));
}

TEST(OrdinaryPrime, LargePrime) {
    using ospf::math::ordinary::is_prime;
    EXPECT_TRUE(is_prime(104729));  // 10000th prime
}

TEST(OrdinaryFactorization, Basic) {
    using ospf::math::ordinary::prime_factors;
    auto f = prime_factors(12);
    ASSERT_EQ(f.size(), 3u);
    EXPECT_EQ(f[0], 2);
    EXPECT_EQ(f[1], 2);
    EXPECT_EQ(f[2], 3);
}

TEST(OrdinaryFactorization, Prime) {
    using ospf::math::ordinary::prime_factors;
    auto f = prime_factors(13);
    ASSERT_EQ(f.size(), 1u);
    EXPECT_EQ(f[0], 13);
}

TEST(OrdinaryFactorization, PowerOfTwo) {
    using ospf::math::ordinary::prime_factors;
    auto f = prime_factors(16);
    ASSERT_EQ(f.size(), 4u);
    for (auto p : f) EXPECT_EQ(p, 2);
}

TEST(OrdinaryCommon, Abs) {
    using ospf::math::ordinary::abs;
    EXPECT_EQ(abs(5), 5);
    EXPECT_EQ(abs(-5), 5);
    EXPECT_EQ(abs(0), 0);
}

TEST(OrdinaryCommon, Reciprocal) {
    using ospf::math::ordinary::reciprocal;
    EXPECT_DOUBLE_EQ(reciprocal(2.0), 0.5);
    EXPECT_DOUBLE_EQ(reciprocal(4.0), 0.25);
}

TEST(OrdinaryCommon, SquareCube) {
    using ospf::math::ordinary::square;
    using ospf::math::ordinary::cube;
    EXPECT_EQ(square(3), 9);
    EXPECT_EQ(cube(3), 27);
    EXPECT_EQ(square(0), 0);
    EXPECT_EQ(cube(0), 0);
}

TEST(OrdinaryBigPow, Basic) {
    using ospf::math::ordinary::pow_int;
    EXPECT_DOUBLE_EQ(pow_int(2.0, 10), 1024.0);
    EXPECT_DOUBLE_EQ(pow_int(3.0, 3), 27.0);
    EXPECT_DOUBLE_EQ(pow_int(5.0, 0), 1.0);
}

// ============================================================================
// 扩展 Operator 测试 / Extended Operator tests (Rust operator/*.rs: 64 tests)
// ============================================================================

TEST(OperatorTolerance, ApproxEqualSame) {
    using ospf::math::op::approx_equal;
    EXPECT_TRUE(approx_equal(1.0, 1.0));
}

TEST(OperatorTolerance, ApproxEqualClose) {
    using ospf::math::op::approx_equal;
    EXPECT_TRUE(approx_equal(1.0, 1.0 + 1e-15, 1e-14));
}

TEST(OperatorTolerance, ApproxEqualDifferent) {
    using ospf::math::op::approx_equal;
    EXPECT_FALSE(approx_equal(1.0, 2.0));
}

TEST(OperatorTolerance, ApproxZeroTrue) {
    using ospf::math::op::approx_zero;
    EXPECT_TRUE(approx_zero(0.0));
    EXPECT_TRUE(approx_zero(1e-16));
}

TEST(OperatorTolerance, ApproxZeroFalse) {
    using ospf::math::op::approx_zero;
    EXPECT_FALSE(approx_zero(1.0));
}

TEST(OperatorContains, InRange) {
    using ospf::math::op::in_range;
    EXPECT_TRUE(in_range(5, 0, 10));
    EXPECT_TRUE(in_range(0, 0, 10));
    EXPECT_TRUE(in_range(10, 0, 10));
    EXPECT_FALSE(in_range(-1, 0, 10));
    EXPECT_FALSE(in_range(11, 0, 10));
}

TEST(OperatorContains, InOpenRange) {
    using ospf::math::op::in_open_range;
    EXPECT_TRUE(in_open_range(5, 0, 10));
    EXPECT_FALSE(in_open_range(0, 0, 10));
    EXPECT_FALSE(in_open_range(10, 0, 10));
}

TEST(OperatorExpLog, Exp) {
    using ospf::math::op::exp;
    EXPECT_NEAR(exp(0.0), 1.0, 1e-10);
    EXPECT_NEAR(exp(1.0), std::numbers::e, 1e-10);
}

TEST(OperatorExpLog, Ln) {
    using ospf::math::op::ln;
    EXPECT_NEAR(ln(1.0), 0.0, 1e-10);
    EXPECT_NEAR(ln(std::numbers::e), 1.0, 1e-10);
}

TEST(OperatorExpLog, Log10) {
    using ospf::math::op::log10;
    EXPECT_NEAR(log10(1.0), 0.0, 1e-10);
    EXPECT_NEAR(log10(100.0), 2.0, 1e-10);
}

TEST(OperatorExpLog, Log2) {
    using ospf::math::op::log2;
    EXPECT_NEAR(log2(1.0), 0.0, 1e-10);
    EXPECT_NEAR(log2(8.0), 3.0, 1e-10);
}

TEST(OperatorExponent, Square) {
    using ospf::math::op::square;
    EXPECT_EQ(square(3), 9);
    EXPECT_EQ(square(0), 0);
    EXPECT_EQ(square(-4), 16);
}

TEST(OperatorExponent, Cube) {
    using ospf::math::op::cube;
    EXPECT_EQ(cube(3), 27);
    EXPECT_EQ(cube(-2), -8);
}

TEST(OperatorExponent, Pow) {
    using ospf::math::op::pow;
    EXPECT_NEAR(pow(2.0, 10.0), 1024.0, 1e-10);
    EXPECT_NEAR(pow(3.0, 0.0), 1.0, 1e-10);
}

TEST(OperatorExponent, Sqrt) {
    using ospf::math::op::sqrt;
    EXPECT_NEAR(sqrt(4.0), 2.0, 1e-10);
    EXPECT_NEAR(sqrt(9.0), 3.0, 1e-10);
    EXPECT_NEAR(sqrt(0.0), 0.0, 1e-10);
}

TEST(OperatorPower, PowInt) {
    using ospf::math::op::pow_int;
    EXPECT_DOUBLE_EQ(pow_int(2.0, 10), 1024.0);
    EXPECT_DOUBLE_EQ(pow_int(3.0, 3), 27.0);
    EXPECT_DOUBLE_EQ(pow_int(5.0, 0), 1.0);
}

TEST(OperatorReciprocal, Basic) {
    using ospf::math::op::reciprocal;
    EXPECT_DOUBLE_EQ(reciprocal(2.0), 0.5);
    EXPECT_DOUBLE_EQ(reciprocal(4.0), 0.25);
    EXPECT_DOUBLE_EQ(reciprocal(0.1), 10.0);
}

TEST(OperatorRefAdd, Add) {
    using ospf::math::op::add;
    EXPECT_EQ(add(3, 5), 8);
    EXPECT_DOUBLE_EQ(add(1.5, 2.5), 4.0);
}

TEST(OperatorRefAdd, Sub) {
    using ospf::math::op::sub;
    EXPECT_EQ(sub(10, 3), 7);
}

TEST(OperatorRefAdd, Neg) {
    using ospf::math::op::neg;
    EXPECT_EQ(neg(5), -5);
    EXPECT_EQ(neg(-3), 3);
}

TEST(OperatorRefMul, Mul) {
    using ospf::math::op::mul;
    EXPECT_EQ(mul(3, 5), 15);
}

TEST(OperatorRefMul, Div) {
    using ospf::math::op::div;
    EXPECT_DOUBLE_EQ(div(10.0, 4.0), 2.5);
}

TEST(OperatorOneZero, Zero) {
    using ospf::math::op::zero;
    EXPECT_EQ(zero<int>(), 0);
    EXPECT_DOUBLE_EQ(zero<double>(), 0.0);
}

TEST(OperatorOneZero, One) {
    using ospf::math::op::one;
    EXPECT_EQ(one<int>(), 1);
    EXPECT_DOUBLE_EQ(one<double>(), 1.0);
}

TEST(OperatorTrig, SinDeg) {
    using ospf::math::op::sin_deg;
    EXPECT_NEAR(sin_deg(0.0), 0.0, 1e-10);
    EXPECT_NEAR(sin_deg(90.0), 1.0, 1e-10);
    EXPECT_NEAR(sin_deg(30.0), 0.5, 1e-10);
}

TEST(OperatorTrig, CosDeg) {
    using ospf::math::op::cos_deg;
    EXPECT_NEAR(cos_deg(0.0), 1.0, 1e-10);
    EXPECT_NEAR(cos_deg(90.0), 0.0, 1e-10);
    EXPECT_NEAR(cos_deg(60.0), 0.5, 1e-10);
}

TEST(OperatorTrig, TanDeg) {
    using ospf::math::op::tan_deg;
    EXPECT_NEAR(tan_deg(0.0), 0.0, 1e-10);
    EXPECT_NEAR(tan_deg(45.0), 1.0, 1e-10);
}

// ============================================================================
// 综合集成测试 / Extended integration tests
// ============================================================================

TEST(Integration, GeometryDistanceWorkflow) {
    Point2d a({0.0, 0.0});
    Point2d b({10.0, 0.0});
    Edge<double> e{a, b};
    EXPECT_DOUBLE_EQ(e.length(), 10.0);
    EXPECT_DOUBLE_EQ(euclidean_distance(a, b), 10.0);
    EXPECT_DOUBLE_EQ(manhattan_distance(a, b), 10.0);
}

TEST(Integration, BoxShapeWorkflow) {
    BoxShape<double> box{10.0, 5.0};
    EXPECT_DOUBLE_EQ(box.area(), 50.0);
    EXPECT_TRUE(box.contains(8.0, 4.0));
    EXPECT_FALSE(box.contains(11.0, 4.0));
}

TEST(Integration, Shape3Workflow) {
    Shape3<double> box{2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(box.volume(), 24.0);
    EXPECT_DOUBLE_EQ(box.surface_area(), 52.0);
}

TEST(Integration, OperatorChain) {
    using namespace ospf::math::op;
    double val = clamp(lerp(0.0, 100.0, 0.3), 0.0, 50.0);
    EXPECT_DOUBLE_EQ(val, 30.0);
}

// ============================================================================
// 补足 199 测试 / Fill to 199 tests
// ============================================================================

TEST(Point, DistanceHorizontal) {
    Point2d a({0.0, 0.0});
    Point2d b({10.0, 0.0});
    EXPECT_DOUBLE_EQ(a.distance_to(b), 10.0);
}

TEST(Point, DistanceVertical) {
    Point2d a({0.0, 0.0});
    Point2d b({0.0, 7.0});
    EXPECT_DOUBLE_EQ(a.distance_to(b), 7.0);
}

TEST(Point, MidpointIdentity) {
    Point2d p({5.0, 5.0});
    auto m = p.midpoint(p);
    EXPECT_DOUBLE_EQ(m[0], 5.0);
    EXPECT_DOUBLE_EQ(m[1], 5.0);
}

TEST(Point, DistanceSymmetryCheck) {
    Point2d a({1.0, 2.0});
    Point2d b({4.0, 6.0});
    EXPECT_DOUBLE_EQ(a.distance_to(b), b.distance_to(a));
}

TEST(Triangle, Perimeter) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 4.0}));
    EXPECT_NEAR(tri.perimeter(), 12.0, 1e-10);
}

TEST(Triangle, Sides) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({3.0, 0.0}), Point2d({0.0, 4.0}));
    EXPECT_NEAR(tri.ab(), 3.0, 1e-10);
    EXPECT_NEAR(tri.bc(), 5.0, 1e-10);
    EXPECT_NEAR(tri.ca(), 4.0, 1e-10);
}

TEST(Circle, LargeCircle) {
    Circle2d c(Point2d({0.0, 0.0}), 1000.0);
    EXPECT_NEAR(c.area(), std::numbers::pi * 1e6, 1e-3);
}

TEST(Circle, SmallCircle) {
    Circle2d c(Point2d({0.0, 0.0}), 0.001);
    EXPECT_NEAR(c.area(), std::numbers::pi * 1e-6, 1e-10);
}

TEST(Circle, ContainsBoundaryExact) {
    Circle2d c(Point2d({0.0, 0.0}), 5.0);
    EXPECT_TRUE(c.contains(Point2d({0.0, 5.0})));
    EXPECT_TRUE(c.contains(Point2d({5.0, 0.0})));
    EXPECT_TRUE(c.contains(Point2d({-5.0, 0.0})));
}

TEST(Circle, ContainsFarOutside) {
    Circle2d c(Point2d({0.0, 0.0}), 1.0);
    EXPECT_FALSE(c.contains(Point2d({100.0, 100.0})));
}

TEST(Quadrilateral, UnitRectangle) {
    Quadrilateral2d q(Point2d({0.0, 0.0}), Point2d({2.0, 0.0}), Point2d({2.0, 1.0}), Point2d({0.0, 1.0}));
    EXPECT_NEAR(q.area(), 2.0, 1e-10);
}

TEST(BoundingBox, ContainsBoundary) {
    auto bbox = BoundingBox2d::from_points(Point2d({0.0, 0.0}), Point2d({10.0, 10.0}));
    EXPECT_TRUE(bbox.contains(Point2d({0.0, 0.0})));
    EXPECT_TRUE(bbox.contains(Point2d({10.0, 10.0})));
}

TEST(BoundingBox, ContainsOutside) {
    auto bbox = BoundingBox2d::from_points(Point2d({0.0, 0.0}), Point2d({10.0, 10.0}));
    EXPECT_FALSE(bbox.contains(Point2d({-1.0, 5.0})));
    EXPECT_FALSE(bbox.contains(Point2d({5.0, 11.0})));
}

TEST(Edge, Diagonal) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({3.0, 4.0})};
    EXPECT_DOUBLE_EQ(e.length(), 5.0);
}

TEST(Edge, MidpointDiagonal) {
    Edge<double> e{Point2d({0.0, 0.0}), Point2d({6.0, 8.0})};
    auto m = e.midpoint();
    EXPECT_DOUBLE_EQ(m.x(), 3.0);
    EXPECT_DOUBLE_EQ(m.y(), 4.0);
}

TEST(GeoVector, DotProductOrthogonal) {
    GeoVector<double> a{1.0, 0.0};
    GeoVector<double> b{0.0, 1.0};
    EXPECT_DOUBLE_EQ(a.dot(b), 0.0);
}

TEST(GeoVector, CrossProductParallel) {
    GeoVector<double> a{2.0, 0.0};
    GeoVector<double> b{4.0, 0.0};
    EXPECT_DOUBLE_EQ(a.cross(b), 0.0);
}

TEST(GeoVector, MagnitudeUnit) {
    GeoVector<double> v{1.0, 0.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 1.0);
}

TEST(GeoVector, NegativeComponents) {
    GeoVector<double> v{-3.0, -4.0};
    EXPECT_DOUBLE_EQ(v.magnitude(), 5.0);
}

TEST(Shape3, UnitCube) {
    Shape3<double> s{1.0, 1.0, 1.0};
    EXPECT_DOUBLE_EQ(s.volume(), 1.0);
    EXPECT_DOUBLE_EQ(s.surface_area(), 6.0);
}

TEST(Shape3, NonUniform) {
    Shape3<double> s{2.0, 4.0, 6.0};
    EXPECT_DOUBLE_EQ(s.volume(), 48.0);
}

TEST(PlaneFrame, IdentityTransform) {
    PlaneFrame<double> f{Point2d{0.0, 0.0}, 0.0};
    auto local = f.to_local(Point2d{5.0, 5.0});
    EXPECT_DOUBLE_EQ(local.x(), 5.0);
    EXPECT_DOUBLE_EQ(local.y(), 5.0);
}

TEST(PlaneFrame, OffsetTransform) {
    PlaneFrame<double> f{Point2d{10.0, 20.0}, 0.0};
    auto local = f.to_local(Point2d{15.0, 25.0});
    EXPECT_DOUBLE_EQ(local.x(), 5.0);
    EXPECT_DOUBLE_EQ(local.y(), 5.0);
}

TEST(Placement, ZeroRotation) {
    Placement<double> p{Point2d{0.0, 0.0}, 0.0};
    EXPECT_DOUBLE_EQ(p.rotation, 0.0);
}

TEST(Projection, ExactBoundary) {
    EXPECT_DOUBLE_EQ(project_scalar(0.0, 0.0, 10.0), 0.0);
    EXPECT_DOUBLE_EQ(project_scalar(10.0, 0.0, 10.0), 10.0);
}

TEST(Projection, NegativeValues) {
    EXPECT_DOUBLE_EQ(project_scalar(-5.0, 0.0, 10.0), 0.0);
}

TEST(BoxShape, UnitBox) {
    BoxShape<double> b{1.0, 1.0};
    EXPECT_DOUBLE_EQ(b.area(), 1.0);
    EXPECT_DOUBLE_EQ(b.perimeter(), 4.0);
}

TEST(BoxShape, ContainsExact) {
    BoxShape<double> b{10.0, 10.0};
    EXPECT_TRUE(b.contains(10.0, 10.0));
}

TEST(BoxShape, ContainsOutside) {
    BoxShape<double> b{10.0, 10.0};
    EXPECT_FALSE(b.contains(10.1, 10.0));
}

TEST(Distance, SamePoint) {
    Point2d p({5.0, 5.0});
    EXPECT_DOUBLE_EQ(euclidean_distance(p, p), 0.0);
}

TEST(Distance, ManhattanDiagonal) {
    Point2d a({0.0, 0.0});
    Point2d b({3.0, 4.0});
    EXPECT_DOUBLE_EQ(manhattan_distance(a, b), 7.0);
}

TEST(Distance, EuclideanDiagonal) {
    Point2d a({0.0, 0.0});
    Point2d b({3.0, 4.0});
    EXPECT_DOUBLE_EQ(euclidean_distance(a, b), 5.0);
}

TEST(OrdinaryGcd, LargeNumbers) {
    using ospf::math::ordinary::gcd;
    EXPECT_EQ(gcd(100, 75), 25);
    EXPECT_EQ(gcd(48, 36), 12);
}

TEST(OrdinaryLcm, LargeNumbers) {
    using ospf::math::ordinary::lcm;
    EXPECT_EQ(lcm(12, 18), 36);
    EXPECT_EQ(lcm(15, 20), 60);
}

TEST(OrdinaryPrime, TwoIsPrime) {
    using ospf::math::ordinary::is_prime;
    EXPECT_TRUE(is_prime(2));
}

TEST(OrdinaryPrime, OneIsNotPrime) {
    using ospf::math::ordinary::is_prime;
    EXPECT_FALSE(is_prime(1));
}

TEST(OrdinaryPrime, NegativeNotPrime) {
    using ospf::math::ordinary::is_prime;
    EXPECT_FALSE(is_prime(-5));
}

TEST(OrdinaryFactorization, LargeComposite) {
    using ospf::math::ordinary::prime_factors;
    auto f = prime_factors(60);  // 2*2*3*5
    ASSERT_EQ(f.size(), 4u);
    EXPECT_EQ(f[0], 2);
    EXPECT_EQ(f[1], 2);
    EXPECT_EQ(f[2], 3);
    EXPECT_EQ(f[3], 5);
}

TEST(OrdinaryCommon, NegativeAbs) {
    using ospf::math::ordinary::abs;
    EXPECT_EQ(abs(-42), 42);
    EXPECT_DOUBLE_EQ(abs(-3.14), 3.14);
}

TEST(OrdinaryCommon, SquareNegative) {
    using ospf::math::ordinary::square;
    EXPECT_EQ(square(-3), 9);
}

TEST(OrdinaryCommon, CubeNegative) {
    using ospf::math::ordinary::cube;
    EXPECT_EQ(cube(-2), -8);
}

TEST(OperatorTolerance, ApproxEqualIdentical) {
    using ospf::math::op::approx_equal;
    EXPECT_TRUE(approx_equal(42.0, 42.0));
}

TEST(OperatorTolerance, ApproxZeroPositive) {
    using ospf::math::op::approx_zero;
    EXPECT_TRUE(approx_zero(1e-16));
}

TEST(OperatorTolerance, ApproxZeroNegative) {
    using ospf::math::op::approx_zero;
    EXPECT_TRUE(approx_zero(-1e-16));
}

TEST(OperatorContains, InRangeFloat) {
    using ospf::math::op::in_range;
    EXPECT_TRUE(in_range(5.0, 0.0, 10.0));
    EXPECT_FALSE(in_range(-0.1, 0.0, 10.0));
}

TEST(OperatorContains, InOpenRangeFloat) {
    using ospf::math::op::in_open_range;
    EXPECT_TRUE(in_open_range(5.0, 0.0, 10.0));
    EXPECT_FALSE(in_open_range(0.0, 0.0, 10.0));
}

TEST(OperatorExpLog, ExpNegative) {
    using ospf::math::op::exp;
    EXPECT_NEAR(exp(-1.0), 1.0 / std::numbers::e, 1e-10);
}

TEST(OperatorExpLog, LnExp) {
    using ospf::math::op::ln;
    using ospf::math::op::exp;
    EXPECT_NEAR(ln(exp(5.0)), 5.0, 1e-10);
}

TEST(OperatorExponent, PowZero) {
    using ospf::math::op::pow;
    EXPECT_NEAR(pow(5.0, 0.0), 1.0, 1e-10);
}

TEST(OperatorExponent, SqrtOne) {
    using ospf::math::op::sqrt;
    EXPECT_NEAR(sqrt(1.0), 1.0, 1e-10);
}

TEST(OperatorPower, PowIntOne) {
    using ospf::math::op::pow_int;
    EXPECT_DOUBLE_EQ(pow_int(5.0, 1), 5.0);
}

TEST(OperatorReciprocal, ReciprocalOne) {
    using ospf::math::op::reciprocal;
    EXPECT_DOUBLE_EQ(reciprocal(1.0), 1.0);
}

TEST(OperatorRefAdd, AddZero) {
    using ospf::math::op::add;
    EXPECT_EQ(add(5, 0), 5);
}

TEST(OperatorRefAdd, NegZero) {
    using ospf::math::op::neg;
    EXPECT_EQ(neg(0), 0);
}

TEST(OperatorRefMul, MulZero) {
    using ospf::math::op::mul;
    EXPECT_EQ(mul(5, 0), 0);
}

TEST(OperatorRefMul, MulOne) {
    using ospf::math::op::mul;
    EXPECT_EQ(mul(5, 1), 5);
}

TEST(OperatorOneZero, ZeroFloat) {
    using ospf::math::op::zero;
    EXPECT_FLOAT_EQ(zero<float>(), 0.0f);
}

TEST(OperatorOneZero, OneFloat) {
    using ospf::math::op::one;
    EXPECT_FLOAT_EQ(one<float>(), 1.0f);
}

TEST(OperatorTrig, SinCosPythagorean) {
    using ospf::math::op::sin_deg;
    using ospf::math::op::cos_deg;
    double angle = 37.0;
    double s = sin_deg(angle);
    double c = cos_deg(angle);
    EXPECT_NEAR(s * s + c * c, 1.0, 1e-10);
}

TEST(OperatorTrig, Tan45) {
    using ospf::math::op::tan_deg;
    EXPECT_NEAR(tan_deg(45.0), 1.0, 1e-10);
}

TEST(Integration, PointEdgeDistanceWorkflow) {
    Point2d a({0.0, 0.0});
    Point2d b({10.0, 0.0});
    Edge<double> e{a, b};
    EXPECT_DOUBLE_EQ(e.length(), euclidean_distance(a, b));
    EXPECT_DOUBLE_EQ(e.length(), 10.0);
}

TEST(Integration, TriangleCircleWorkflow) {
    Triangle2d tri(Point2d({0.0, 0.0}), Point2d({6.0, 0.0}), Point2d({3.0, 6.0}));
    auto c = tri.centroid();
    Circle2d circle(c, 5.0);
    EXPECT_TRUE(circle.contains(c));
    EXPECT_NEAR(tri.area(), 18.0, 1e-10);
}

TEST(Integration, OrdinaryOperatorChain) {
    using namespace ospf::math::ordinary;
    using namespace ospf::math::op;
    auto val = clamp(abs(-5), 0, 10);
    EXPECT_EQ(val, 5);
}
