#pragma once
/// RabinovichFabrikant 混沌系统 / RabinovichFabrikant chaotic system
/// 1:1 对应 Rust chaotic/rabinovich_fabrikant.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct RabinovichFabrikantParams {
        double a = 1.1;
        double b = 0.87;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y*(z - S{1} + x*x) + S{a}*x, x*(z - S{1} + x*x) + S{a}*y, -S{b}*z + x*y};
        }
    };

    static constexpr char kRabinovichFabrikantName[] = "rabinovich_fabrikant";
    using RabinovichFabrikantSystem = ChaoticSystem3D<RabinovichFabrikantParams, kRabinovichFabrikantName>;

}  // namespace ospf::math::chaotic
