#pragma once
/// 放置策略 / Placement strategy /// 1:1 对应 Rust geometry/placement.rs
#include <ospf/math/geometry/point.hpp>
namespace ospf::math::geometry {
    template<typename S = double>
    struct Placement {
        Point<S, 2> position;
        S rotation;
        [[nodiscard]] bool operator==(const Placement&) const = default;
    };
}
