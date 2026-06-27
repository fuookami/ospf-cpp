#pragma once
/// DoublePendulum 混沌系统 / DoublePendulum chaotic system
/// 1:1 对应 Rust chaotic/double_pendulum.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct DoublePendulumParams {
        double g = 9.81;
        double l = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -S{g}/S{l}*std::sin(x)};
        }
    };

    static constexpr char kDoublePendulumName[] = "double_pendulum";
    using DoublePendulumSystem = ChaoticSystem2D<DoublePendulumParams, kDoublePendulumName>;

}  // namespace ospf::math::chaotic
