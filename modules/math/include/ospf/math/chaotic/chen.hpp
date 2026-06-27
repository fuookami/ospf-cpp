#pragma once
/// Chen 混沌系统 / Chen chaotic system
/// 1:1 对应 Rust chaotic/chen.rs
///
/// 方程 / Equations:
/// dx/dt = a * (y - x)
/// dy/dt = (c - a) * x - x*z + c*y
/// dz/dt = x*y - b*z
///
/// 参数 / Parameters: a=35, b=3, c=28

#include <ospf/math/chaotic/chaotic_factory.hpp>

namespace ospf::math::chaotic {

    struct ChenParams {
        double a = 35.0;
        double b = 3.0;
        double c = 28.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            S dx = S{a} * (y - x);
            S dy = (S{c} - S{a}) * x - x * z + S{c} * y;
            S dz = x * y - S{b} * z;
            return {dx, dy, dz};
        }
    };

    static constexpr char kChenName[] = "chen";
    using ChenSystem = ChaoticSystem3D<ChenParams, kChenName>;

}  // namespace ospf::math::chaotic
