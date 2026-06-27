#pragma once
/// Coullet 混沌系统 / Coullet chaotic system
/// 1:1 对应 Rust chaotic/coullet.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct CoulletParams {
        double a = 0.8;
        double b = -1.1;
        double c = -1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y, z, -S{a}*x - S{b}*y - S{c}*z - x*x*x};
        }
    };

    static constexpr char kCoulletName[] = "coullet";
    using CoulletSystem = ChaoticSystem3D<CoulletParams, kCoulletName>;

}  // namespace ospf::math::chaotic
