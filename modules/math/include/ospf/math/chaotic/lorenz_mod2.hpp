#pragma once
/// LorenzMod2 混沌系统 / LorenzMod2 chaotic system
/// 1:1 对应 Rust chaotic/lorenz_mod2.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LorenzMod2Params {
        double a = 10.0;
        double b = 28.0;
        double c = 2.6666666666666665;
        double d = 0.5;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x) + S{d}*y*z, S{b}*x - y - x*z, x*y - S{c}*z};
        }
    };

    static constexpr char kLorenzMod2Name[] = "lorenz_mod2";
    using LorenzMod2System = ChaoticSystem3D<LorenzMod2Params, kLorenzMod2Name>;

}  // namespace ospf::math::chaotic
