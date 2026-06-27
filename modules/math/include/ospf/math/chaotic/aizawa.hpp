#pragma once
/// Aizawa 混沌系统 / Aizawa chaotic system
/// 1:1 对应 Rust chaotic/aizawa.rs
///
/// 方程 / Equations:
/// dx/dt = (z - b)*x - d*y
/// dy/dt = d*x + (z - b)*y
/// dz/dt = c + a*z - z^3/3 - (x^2 + y^2)*(1 + e*z) + f*z*x^3
///
/// 参数 / Parameters: a=0.95, b=0.7, c=0.6, d=3.5, e=0.25, f=0.1

#include <ospf/math/chaotic/chaotic_factory.hpp>

namespace ospf::math::chaotic {

    struct AizawaParams {
        double a = 0.95;
        double b = 0.7;
        double c = 0.6;
        double d = 3.5;
        double e = 0.25;
        double f = 0.1;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            S dx = (z - S{b}) * x - S{d} * y;
            S dy = S{d} * x + (z - S{b}) * y;
            S dz = S{c} + S{a} * z - (z * z * z) / S{3}
                   - (x * x + y * y) * (S{1} + S{e} * z)
                   + S{f} * z * x * x * x;
            return {dx, dy, dz};
        }
    };

    static constexpr char kAizawaName[] = "aizawa";
    using AizawaSystem = ChaoticSystem3D<AizawaParams, kAizawaName>;

}  // namespace ospf::math::chaotic
