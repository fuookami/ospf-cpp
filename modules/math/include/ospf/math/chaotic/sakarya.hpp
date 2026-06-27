#pragma once
/// Sakarya 混沌系统 / Sakarya chaotic system
/// 1:1 对应 Rust chaotic/sakarya.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct SakaryaParams {
        double a = 0.4;
        double b = 0.3;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{a}*x + y*z, -S{b}*y + x*z, S{1} - x*y};
        }
    };

    static constexpr char kSakaryaName[] = "sakarya";
    using SakaryaSystem = ChaoticSystem3D<SakaryaParams, kSakaryaName>;

}  // namespace ospf::math::chaotic
