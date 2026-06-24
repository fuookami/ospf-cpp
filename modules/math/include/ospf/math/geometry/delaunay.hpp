#pragma once
/// Delaunay 三角剖分 / Delaunay triangulation /// 1:1 对应 Rust geometry/delaunay.rs
#include <ospf/math/geometry/point.hpp>
#include <ospf/math/geometry/triangle.hpp>
#include <vector>
namespace ospf::math::geometry {
    template<typename S = double>
    struct DelaunayTriangulation {
        std::vector<Triangle<S>> triangles;
        [[nodiscard]] std::size_t size() const { return triangles.size(); }
    };
}
