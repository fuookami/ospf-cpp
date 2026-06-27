#pragma once
/// ChuaCircuit 混沌系统 / ChuaCircuit chaotic system
/// 1:1 对应 Rust chaotic/chua_circuit.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ChuaCircuitParams {
        double a = 15.6;
        double b = 28.0;
        double c = -1.143;
        double d = -0.714;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x - S{c}*x - S{d}*x*x*x), S{b}*(x - y) + z, -S{1.0}*y};
        }
    };

    static constexpr char kChuaCircuitName[] = "chua_circuit";
    using ChuaCircuitSystem = ChaoticSystem3D<ChuaCircuitParams, kChuaCircuitName>;

}  // namespace ospf::math::chaotic
