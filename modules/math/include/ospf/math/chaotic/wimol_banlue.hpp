#pragma once
/// WimolBanlue 混沌系统 / WimolBanlue chaotic system
/// 1:1 对应 Rust chaotic/wimol_banlue.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct WimolBanlueParams {
        double a = 1.0;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{a}*x + y*z, -S{b}*y + x*z, S{1} - x*y};
        }
    };

    static constexpr char kWimolBanlueName[] = "wimol_banlue";
    using WimolBanlueSystem = ChaoticSystem3D<WimolBanlueParams, kWimolBanlueName>;

}  // namespace ospf::math::chaotic
