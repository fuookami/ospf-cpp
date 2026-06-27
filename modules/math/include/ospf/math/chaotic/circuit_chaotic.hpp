#pragma once
/// CircuitChaotic 混沌系统 / CircuitChaotic chaotic system
/// 1:1 对应 Rust chaotic/circuit_chaotic.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct CircuitChaoticParams {
        double a = 1.0;
        double b = 1.0;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), (S{b}-S{a})*x - x*z + S{b}*y, x*y - S{c}*z};
        }
    };

    static constexpr char kCircuitChaoticName[] = "circuit_chaotic";
    using CircuitChaoticSystem = ChaoticSystem3D<CircuitChaoticParams, kCircuitChaoticName>;

}  // namespace ospf::math::chaotic
