#pragma once
/// 边 / Edge /// 1:1 对应 Rust geometry/edge.rs
#include <ospf/math/geometry/point.hpp>
#include <cstddef>
namespace ospf::math::geometry {
    template<typename S = double>
    struct Edge {
        Point<S, 2> start;
        Point<S, 2> end;
        [[nodiscard]] S length() const { return start.distance_to(end); }
        [[nodiscard]] Point<S, 2> midpoint() const {
            return Point<S, 2>{(start.x() + end.x()) / S{2}, (start.y() + end.y()) / S{2}};
        }
        [[nodiscard]] bool operator==(const Edge&) const = default;
    };
}
