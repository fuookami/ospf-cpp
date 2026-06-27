#pragma once
/// FourScrollHyperChaotic 混沌系统 / FourScrollHyperChaotic chaotic system
/// 1:1 对应 Rust chaotic/four_scroll_hyper_chaotic.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct FourScrollHyperChaoticParams {
        double a = 10.0;
        double b = 40.0;
        double c = 2.0;
        double d = 2.5;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), S{b}*x - S{c}*y - x*z, x*y - S{d}*z};
        }
    };

    static constexpr char kFourScrollHyperChaoticName[] = "four_scroll_hyper_chaotic";
    using FourScrollHyperChaoticSystem = ChaoticSystem3D<FourScrollHyperChaoticParams, kFourScrollHyperChaoticName>;

}  // namespace ospf::math::chaotic
