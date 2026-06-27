#pragma once
/// Hadley 混沌系统 / Hadley chaotic system
/// 1:1 对应 Rust chaotic/hadley.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct HadleyParams {
        double a = 0.2;
        double b = 4.0;
        double c = 8.0;
        double d = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-y*y - z*z - S{a}*x + S{a}*S{c}, x*y - S{b}*x*z - y + S{d}, S{b}*x*y + x*z - z};
        }
    };

    static constexpr char kHadleyName[] = "hadley";
    using HadleySystem = ChaoticSystem3D<HadleyParams, kHadleyName>;

}  // namespace ospf::math::chaotic
