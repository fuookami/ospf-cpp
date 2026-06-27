#pragma once
/// LorenzMod1 混沌系统 / LorenzMod1 chaotic system
/// 1:1 对应 Rust chaotic/lorenz_mod1.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LorenzMod1Params {
        double a = 10.0;
        double b = 28.0;
        double c = 2.6666666666666665;
        double d = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x) + S{d}*y*z, S{b}*x - y - x*z, x*y - S{c}*z};
        }
    };

    static constexpr char kLorenzMod1Name[] = "lorenz_mod1";
    using LorenzMod1System = ChaoticSystem3D<LorenzMod1Params, kLorenzMod1Name>;

}  // namespace ospf::math::chaotic
