#pragma once
/// LorenzStenflo 混沌系统 / LorenzStenflo chaotic system
/// 4D 系统 / 4D system

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LorenzStenfloParams {
        double a = 10.0;
        double b = 28.0;
        double c = 8.0/3.0;
        double d = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), S{b}*x - y - x*z, x*y - S{c}*z};
        }
    };

    static constexpr char kLorenzStenfloName[] = "lorenz_stenflo";
    using LorenzStenfloSystem = ChaoticSystem3D<LorenzStenfloParams, kLorenzStenfloName>;

}  // namespace ospf::math::chaotic
