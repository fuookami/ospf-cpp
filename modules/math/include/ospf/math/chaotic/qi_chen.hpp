#pragma once
/// QiChen 混沌系统 / QiChen chaotic system
/// 1:1 对应 Rust chaotic/qi_chen.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct QiChenParams {
        double a = 38.0;
        double b = 2.0;
        double c = 26.666666666666668;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x) + x*z, S{b}*x - y - x*z, -S{c}*z + x*y};
        }
    };

    static constexpr char kQiChenName[] = "qi_chen";
    using QiChenSystem = ChaoticSystem3D<QiChenParams, kQiChenName>;

}  // namespace ospf::math::chaotic
