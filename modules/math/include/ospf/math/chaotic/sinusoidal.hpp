#pragma once
/// Sinusoidal 混沌系统 / Sinusoidal chaotic system
/// 1:1 对应 Rust chaotic/sinusoidal.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct SinusoidalParams {
        double a = 1.0;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -S{a}*std::sin(x) - S{b}*y};
        }
    };

    static constexpr char kSinusoidalName[] = "sinusoidal";
    using SinusoidalSystem = ChaoticSystem2D<SinusoidalParams, kSinusoidalName>;

}  // namespace ospf::math::chaotic
