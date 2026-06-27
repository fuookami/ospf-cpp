#pragma once
/// RayleighBenard 混沌系统 / RayleighBenard chaotic system
/// 1:1 对应 Rust chaotic/rayleigh_benard.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct RayleighBenardParams {
        double r = 28.0;
        double b = 2.6666666666666665;
        double p = 10.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{p}*(y - x), S{r}*x - y - x*z, x*y - S{b}*z};
        }
    };

    static constexpr char kRayleighBenardName[] = "rayleigh_benard";
    using RayleighBenardSystem = ChaoticSystem3D<RayleighBenardParams, kRayleighBenardName>;

}  // namespace ospf::math::chaotic
