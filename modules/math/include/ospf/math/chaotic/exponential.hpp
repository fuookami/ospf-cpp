#pragma once
/// Exponential 混沌系统 / Exponential chaotic system
/// 1:1 对应 Rust chaotic/exponential.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ExponentialParams {
        double a = 1.0;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -S{a}*x + S{b}*y*(S{1} - x*x)};
        }
    };

    static constexpr char kExponentialName[] = "exponential";
    using ExponentialSystem = ChaoticSystem2D<ExponentialParams, kExponentialName>;

}  // namespace ospf::math::chaotic
