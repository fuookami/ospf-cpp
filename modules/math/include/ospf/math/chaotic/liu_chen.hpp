#pragma once
/// LiuChen 混沌系统 / LiuChen chaotic system
/// 1:1 对应 Rust chaotic/liu_chen.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LiuChenParams {
        double a = 2.0;
        double b = 1.0;
        double c = 4.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), (S{c}-S{a})*x - x*z + S{c}*y, x*y - S{b}*z};
        }
    };

    static constexpr char kLiuChenName[] = "liu_chen";
    using LiuChenSystem = ChaoticSystem3D<LiuChenParams, kLiuChenName>;

}  // namespace ospf::math::chaotic
