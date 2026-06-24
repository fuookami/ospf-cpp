#pragma once
/// IntervalExchange 混沌系统 / IntervalExchange chaotic system
/// 1:1 对应 Rust chaotic/interval_exchange.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct IntervalExchangeParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kIntervalExchangeName[] = "interval_exchange";
    using IntervalExchangeSystem = ChaoticSystem3D<IntervalExchangeParams, kIntervalExchangeName>;
}
