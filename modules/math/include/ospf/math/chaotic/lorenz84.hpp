#pragma once
/// Lorenz84 混沌系统 / Lorenz84 chaotic system
/// 1:1 对应 Rust chaotic/lorenz84.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct Lorenz84Params {
        double a = 0.95;
        double b = 7.91;
        double c = 4.83;
        double d = 4.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {-S{a}*x - y*y - z*z + S{a}*S{c}, -y + x*y - S{b}*x*z + S{d}, -z + S{b}*x*y + x*z};
        }
    };

    static constexpr char kLorenz84Name[] = "lorenz84";
    using Lorenz84System = ChaoticSystem3D<Lorenz84Params, kLorenz84Name>;

}  // namespace ospf::math::chaotic
