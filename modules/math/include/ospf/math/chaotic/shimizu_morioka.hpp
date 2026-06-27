#pragma once
/// ShimizuMorioka 混沌系统 / ShimizuMorioka chaotic system
/// 1:1 对应 Rust chaotic/shimizu_morioka.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ShimizuMoriokaParams {
        double a = 0.75;
        double b = 0.45;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y, (S{1}-z)*x - S{a}*y, -S{b}*z + x*x};
        }
    };

    static constexpr char kShimizuMoriokaName[] = "shimizu_morioka";
    using ShimizuMoriokaSystem = ChaoticSystem3D<ShimizuMoriokaParams, kShimizuMoriokaName>;

}  // namespace ospf::math::chaotic
