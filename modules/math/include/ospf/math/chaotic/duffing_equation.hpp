#pragma once
/// DuffingEquation 混沌系统 / DuffingEquation chaotic system
/// 1:1 对应 Rust chaotic/duffing_equation.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct DuffingEquationParams {
        double a = 0.3;
        double b = 0.2;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -S{a}*y - x - S{b}*x*x*x + S{c}*std::cos(y)};
        }
    };

    static constexpr char kDuffingEquationName[] = "duffing_equation";
    using DuffingEquationSystem = ChaoticSystem2D<DuffingEquationParams, kDuffingEquationName>;

}  // namespace ospf::math::chaotic
