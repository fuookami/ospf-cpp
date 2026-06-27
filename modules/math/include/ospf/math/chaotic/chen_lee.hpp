#pragma once
/// ChenLee 混沌系统 / ChenLee chaotic system
/// 1:1 对应 Rust chaotic/chen_lee.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ChenLeeParams {
        double a = 5.0;
        double b = -10.0;
        double c = -0.38;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*x - y*z, S{b}*y + x*z, S{c}*z + x*y/S{3}};
        }
    };

    static constexpr char kChenLeeName[] = "chen_lee";
    using ChenLeeSystem = ChaoticSystem3D<ChenLeeParams, kChenLeeName>;

}  // namespace ospf::math::chaotic
