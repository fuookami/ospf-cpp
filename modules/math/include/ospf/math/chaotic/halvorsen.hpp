#pragma once
/// Halvorsen 混沌系统 / Halvorsen chaotic system
/// 1:1 对应 Rust chaotic/halvorsen.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct HalvorsenParams {
        double a = 1.89;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{a}*x - S{4}*y - S{4}*z - y*y, -S{a}*y - S{4}*z - S{4}*x - z*z, -S{a}*z - S{4}*x - S{4}*y - x*x};
        }
    };

    static constexpr char kHalvorsenName[] = "halvorsen";
    using HalvorsenSystem = ChaoticSystem3D<HalvorsenParams, kHalvorsenName>;

}  // namespace ospf::math::chaotic
