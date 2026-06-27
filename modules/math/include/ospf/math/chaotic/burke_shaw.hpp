#pragma once
/// BurkeShaw 混沌系统 / BurkeShaw chaotic system
/// 1:1 对应 Rust chaotic/burke_shaw.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct BurkeShawParams {
        double s = 10.0;
        double v = 4.267;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{s}*(x+y), -y - S{s}*x*z, S{s}*x*y + S{v}};
        }
    };

    static constexpr char kBurkeShawName[] = "burke_shaw";
    using BurkeShawSystem = ChaoticSystem3D<BurkeShawParams, kBurkeShawName>;

}  // namespace ospf::math::chaotic
