#pragma once
/// NoseHoover 混沌系统 / NoseHoover chaotic system
/// 1:1 对应 Rust chaotic/nose_hoover.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct NoseHooverParams {
        double a = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y, -x + y*z, S{a} - y*y};
        }
    };

    static constexpr char kNoseHooverName[] = "nose_hoover";
    using NoseHooverSystem = ChaoticSystem3D<NoseHooverParams, kNoseHooverName>;

}  // namespace ospf::math::chaotic
