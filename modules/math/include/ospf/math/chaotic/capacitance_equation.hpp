#pragma once
/// CapacitanceEquation 混沌系统 / CapacitanceEquation chaotic system
/// 1:1 对应 Rust chaotic/capacitance_equation.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct CapacitanceEquationParams {
        double a = 1.0;
        double b = 1.0;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), S{b}*x - y - x*z, x*y - S{c}*z};
        }
    };

    static constexpr char kCapacitanceEquationName[] = "capacitance_equation";
    using CapacitanceEquationSystem = ChaoticSystem3D<CapacitanceEquationParams, kCapacitanceEquationName>;

}  // namespace ospf::math::chaotic
