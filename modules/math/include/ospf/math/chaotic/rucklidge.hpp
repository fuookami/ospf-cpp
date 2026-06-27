#pragma once
/// Rucklidge 混沌系统 / Rucklidge chaotic system
/// 1:1 对应 Rust chaotic/rucklidge.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct RucklidgeParams {
        double k = 2.0;
        double a = 6.7;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{k}*x + S{a}*y - y*z, x, -z + y*y};
        }
    };

    static constexpr char kRucklidgeName[] = "rucklidge";
    using RucklidgeSystem = ChaoticSystem3D<RucklidgeParams, kRucklidgeName>;

}  // namespace ospf::math::chaotic
