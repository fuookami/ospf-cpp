#pragma once
/// AnishchenkoAstakhov 混沌系统 / AnishchenkoAstakhov chaotic system
/// 1:1 对应 Rust chaotic/anishchenko_astakhov.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct AnishchenkoAstakhovParams {
        double a = 1.2;
        double b = 0.5;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*x - x*x*x - S{b}*y, x - S{c}*y, S{0.1}*(x - z)};
        }
    };

    static constexpr char kAnishchenkoAstakhovName[] = "anishchenko_astakhov";
    using AnishchenkoAstakhovSystem = ChaoticSystem3D<AnishchenkoAstakhovParams, kAnishchenkoAstakhovName>;

}  // namespace ospf::math::chaotic
