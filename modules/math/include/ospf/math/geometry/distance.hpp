#pragma once
/// 距离计算 / Distance calculations /// 1:1 对应 Rust geometry/distance.rs
#include <ospf/math/geometry/point.hpp>
#include <cmath>
namespace ospf::math::geometry {
    template<typename S = double>
    [[nodiscard]] S euclidean_distance(const Point<S, 2>& a, const Point<S, 2>& b) {
        return a.distance_to(b);
    }
    template<typename S = double>
    [[nodiscard]] S manhattan_distance(const Point<S, 2>& a, const Point<S, 2>& b) {
        return std::abs(a.x() - b.x()) + std::abs(a.y() - b.y());
    }
}
