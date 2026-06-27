#pragma once
/// Singer 混沌系统 / Singer chaotic system
/// 1:1 对应 Rust chaotic/singer.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct SingerParams {
        double a = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*x - y*z, x*z, -z + x*y};
        }
    };

    static constexpr char kSingerName[] = "singer";
    using SingerSystem = ChaoticSystem3D<SingerParams, kSingerName>;

}  // namespace ospf::math::chaotic
