#pragma once
/// GaussIterated 混沌系统 / GaussIterated chaotic system
/// 1:1 对应 Rust chaotic/gauss_iterated.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct GaussIteratedParams {
        double a = 0.5;
        double b = 7.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, S{a}*y*(S{1} - x*x) - x};
        }
    };

    static constexpr char kGaussIteratedName[] = "gauss_iterated";
    using GaussIteratedSystem = ChaoticSystem2D<GaussIteratedParams, kGaussIteratedName>;

}  // namespace ospf::math::chaotic
