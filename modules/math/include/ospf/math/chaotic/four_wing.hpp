#pragma once
/// FourWing 混沌系统 / FourWing chaotic system
/// 1:1 对应 Rust chaotic/four_wing.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct FourWingParams {
        double a = 4.0;
        double b = 6.0;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*x - y*z, S{b}*y - x*z, S{c}*z - x*y};
        }
    };

    static constexpr char kFourWingName[] = "four_wing";
    using FourWingSystem = ChaoticSystem3D<FourWingParams, kFourWingName>;

}  // namespace ospf::math::chaotic
