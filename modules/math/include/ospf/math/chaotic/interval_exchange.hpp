#pragma once
/// IntervalExchange 混沌系统 / IntervalExchange chaotic system
/// 1:1 对应 Rust chaotic/interval_exchange.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct IntervalExchangeParams {
        double a = 1.0;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -x + S{a}*y*(S{1} - x*x)};
        }
    };

    static constexpr char kIntervalExchangeName[] = "interval_exchange";
    using IntervalExchangeSystem = ChaoticSystem2D<IntervalExchangeParams, kIntervalExchangeName>;

}  // namespace ospf::math::chaotic
