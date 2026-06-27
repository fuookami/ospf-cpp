#pragma once
/// ArnoldTongue 混沌系统 / ArnoldTongue chaotic system
/// 1:1 对应 Rust chaotic/arnold_tongue.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ArnoldTongueParams {
        double a = 0.5;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{a}*x + y, S{b}*y - x*x};
        }
    };

    static constexpr char kArnoldTongueName[] = "arnold_tongue";
    using ArnoldTongueSystem = ChaoticSystem2D<ArnoldTongueParams, kArnoldTongueName>;

}  // namespace ospf::math::chaotic
