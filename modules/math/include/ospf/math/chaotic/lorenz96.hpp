#pragma once
/// Lorenz96 混沌系统 / Lorenz96 chaotic system
/// 1:1 对应 Rust chaotic/lorenz96.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct Lorenz96Params {
        double F = 8.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {(y - S{10.0}*z)*y - x + S{F}, (z - x)*y - y + S{F}, (x - y)*z - z + S{F}};
        }
    };

    static constexpr char kLorenz96Name[] = "lorenz96";
    using Lorenz96System = ChaoticSystem3D<Lorenz96Params, kLorenz96Name>;

}  // namespace ospf::math::chaotic
