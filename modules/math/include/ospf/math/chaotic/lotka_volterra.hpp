#pragma once
/// LotkaVolterra 混沌系统 / LotkaVolterra chaotic system
/// 1:1 对应 Rust chaotic/lotka_volterra.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LotkaVolterraParams {
        double a = 1.0;
        double b = 0.1;
        double c = 1.5;
        double d = 0.75;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{a}*x - S{b}*x*y, -S{c}*y + S{d}*x*y};
        }
    };

    static constexpr char kLotkaVolterraName[] = "lotka_volterra";
    using LotkaVolterraSystem = ChaoticSystem2D<LotkaVolterraParams, kLotkaVolterraName>;

}  // namespace ospf::math::chaotic
