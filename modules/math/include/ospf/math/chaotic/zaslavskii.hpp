#pragma once
/// Zaslavskii 混沌系统 / Zaslavskii chaotic system
/// 1:1 对应 Rust chaotic/zaslavskii.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ZaslavskiiParams {
        double epsilon = 0.5;
        double r = 1.0;
        double d = 0.7;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {x + S{epsilon}*std::cos(y), y + S{r} - S{d}*x};
        }
    };

    static constexpr char kZaslavskiiName[] = "zaslavskii";
    using ZaslavskiiSystem = ChaoticSystem2D<ZaslavskiiParams, kZaslavskiiName>;

}  // namespace ospf::math::chaotic
