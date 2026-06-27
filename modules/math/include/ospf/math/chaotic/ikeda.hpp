#pragma once
/// Ikeda 混沌系统 / Ikeda chaotic system
/// 1:1 对应 Rust chaotic/ikeda.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct IkedaParams {
        double u = 0.9;
        double k = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            S t = S{0.4} - S{6}/(S{1} + x*x + y*y);
            return {S{1} + S{u}*(x*std::cos(t) - y*std::sin(t)), S{u}*(x*std::sin(t) + y*std::cos(t))};
        }
    };

    static constexpr char kIkedaName[] = "ikeda";
    using IkedaSystem = ChaoticSystem2D<IkedaParams, kIkedaName>;

}  // namespace ospf::math::chaotic
