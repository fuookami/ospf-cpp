#pragma once
/// WangSun 混沌系统 / WangSun chaotic system
/// 1:1 对应 Rust chaotic/wang_sun.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct WangSunParams {
        double a = 10.0;
        double b = 40.0;
        double c = 2.0;
        double d = 2.5;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), S{b}*x - S{c}*y - x*z, x*y - S{d}*z};
        }
    };

    static constexpr char kWangSunName[] = "wang_sun";
    using WangSunSystem = ChaoticSystem3D<WangSunParams, kWangSunName>;

}  // namespace ospf::math::chaotic
