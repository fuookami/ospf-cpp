#pragma once
/// NBody 混沌系统 / NBody chaotic system
/// 1:1 对应 Rust chaotic/n_body.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct NBodyParams {
        double G = 1.0;
        double n = 3.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{G}*x, -S{G}*y, -S{G}*z};
        }
    };

    static constexpr char kNBodyName[] = "n_body";
    using NBodySystem = ChaoticSystem3D<NBodyParams, kNBodyName>;

}  // namespace ospf::math::chaotic
