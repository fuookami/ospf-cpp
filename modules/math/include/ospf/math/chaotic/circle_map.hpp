#pragma once
/// CircleMap 混沌系统 / CircleMap chaotic system
/// 1:1 对应 Rust chaotic/circle_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct CircleMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kCircleMapName[] = "circle_map";
    using CircleMapSystem = ChaoticSystem2D<CircleMapParams, kCircleMapName>;
}
