#pragma once
/// VanDerPol 混沌系统 / VanDerPol chaotic system
/// 1:1 对应 Rust chaotic/van_der_pol.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct VanDerPolParams {
        double mu = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, S{mu}*(S{1} - x*x)*y - x};
        }
    };

    static constexpr char kVanDerPolName[] = "van_der_pol";
    using VanDerPolSystem = ChaoticSystem2D<VanDerPolParams, kVanDerPolName>;

}  // namespace ospf::math::chaotic
