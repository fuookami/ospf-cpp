#pragma once
/// Arneodo 混沌系统 / Arneodo chaotic system
/// 1:1 对应 Rust chaotic/arneodo.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ArneodoParams {
        double a = -5.5;
        double b = 3.5;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y, z, -S{a}*x - S{b}*y - z + S{c}*x*x*x};
        }
    };

    static constexpr char kArneodoName[] = "arneodo";
    using ArneodoSystem = ChaoticSystem3D<ArneodoParams, kArneodoName>;

}  // namespace ospf::math::chaotic
