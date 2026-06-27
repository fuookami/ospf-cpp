#pragma once
/// HindmarshRose 混沌系统 / HindmarshRose chaotic system
/// 1:1 对应 Rust chaotic/hindmarsh_rose.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct HindmarshRoseParams {
        double a = 1.0;
        double b = 3.0;
        double c = 1.0;
        double d = 5.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y - S{a}*x*x*x + S{b}*x*x - z + S{1}, S{c} - S{d}*x*x - y, S{0.001}*(x - z)};
        }
    };

    static constexpr char kHindmarshRoseName[] = "hindmarsh_rose";
    using HindmarshRoseSystem = ChaoticSystem3D<HindmarshRoseParams, kHindmarshRoseName>;

}  // namespace ospf::math::chaotic
