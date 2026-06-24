#pragma once
/// 平面坐标系 / Plane frame /// 1:1 对应 Rust geometry/plane_frame.rs
#include <ospf/math/geometry/point.hpp>
namespace ospf::math::geometry {
    template<typename S = double>
    struct PlaneFrame {
        Point<S, 2> origin;
        S rotation;
        [[nodiscard]] Point<S, 2> to_local(const Point<S, 2>& world) const {
            return Point<S, 2>{world.x() - origin.x(), world.y() - origin.y()};
        }
        [[nodiscard]] bool operator==(const PlaneFrame&) const = default;
    };
}
