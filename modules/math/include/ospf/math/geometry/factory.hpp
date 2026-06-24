#pragma once
/// 几何工厂 / Geometry factory /// 1:1 对应 Rust geometry/factory.rs
#include <ospf/math/geometry/circle.hpp>
#include <ospf/math/geometry/triangle.hpp>
namespace ospf::math::geometry {
    template<typename S = double>
    [[nodiscard]] Circle<S> make_circle(S cx, S cy, S r) { return Circle<S>(Point<S, 2>(cx, cy), r); }
}
