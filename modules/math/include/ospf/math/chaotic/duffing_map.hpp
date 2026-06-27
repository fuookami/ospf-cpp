#pragma once
/// DuffingMap 混沌系统 / DuffingMap chaotic system
/// 1:1 对应 Rust chaotic/duffing_map.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct DuffingMapParams {
        double a = 2.75;
        double b = 0.2;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -S{b}*x + S{a}*y - y*y*y};
        }
    };

    static constexpr char kDuffingMapName[] = "duffing_map";
    using DuffingMapSystem = ChaoticSystem2D<DuffingMapParams, kDuffingMapName>;

}  // namespace ospf::math::chaotic
