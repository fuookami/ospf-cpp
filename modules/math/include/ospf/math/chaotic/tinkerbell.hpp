#pragma once
/// Tinkerbell 混沌系统 / Tinkerbell chaotic system
/// 1:1 对应 Rust chaotic/tinkerbell.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct TinkerbellParams {
        double a = 0.9;
        double b = -0.6013;
        double c = 2.0;
        double d = 0.5;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {x*x - y*y + S{a}*x + S{b}*y, S{2}*x*y + S{c}*x + S{d}*y};
        }
    };

    static constexpr char kTinkerbellName[] = "tinkerbell";
    using TinkerbellSystem = ChaoticSystem2D<TinkerbellParams, kTinkerbellName>;

}  // namespace ospf::math::chaotic
