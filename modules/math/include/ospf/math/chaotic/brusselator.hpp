#pragma once
/// Brusselator 混沌系统 / Brusselator chaotic system
/// 1:1 对应 Rust chaotic/brusselator.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct BrusselatorParams {
        double a = 1.0;
        double b = 3.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{a} + x*x*y - (S{b}+S{1})*x, S{b}*x - x*x*y};
        }
    };

    static constexpr char kBrusselatorName[] = "brusselator";
    using BrusselatorSystem = ChaoticSystem2D<BrusselatorParams, kBrusselatorName>;

}  // namespace ospf::math::chaotic
