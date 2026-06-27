#pragma once
/// NewtonLeipnik 混沌系统 / NewtonLeipnik chaotic system
/// 1:1 对应 Rust chaotic/newton_leipnik.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct NewtonLeipnikParams {
        double a = 0.4;
        double b = 0.175;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{a}*x + y + S{10}*y*z, -x - S{0.4}*y + S{5}*x*z, S{b}*z - S{5}*x*y};
        }
    };

    static constexpr char kNewtonLeipnikName[] = "newton_leipnik";
    using NewtonLeipnikSystem = ChaoticSystem3D<NewtonLeipnikParams, kNewtonLeipnikName>;

}  // namespace ospf::math::chaotic
