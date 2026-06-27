#pragma once
/// KaplanYorke 混沌系统 / KaplanYorke chaotic system
/// 1:1 对应 Rust chaotic/kaplan_yorke.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct KaplanYorkeParams {
        double a = 0.1;
        double b = 0.1;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{a}*x, -y - x*x};
        }
    };

    static constexpr char kKaplanYorkeName[] = "kaplan_yorke";
    using KaplanYorkeSystem = ChaoticSystem2D<KaplanYorkeParams, kKaplanYorkeName>;

}  // namespace ospf::math::chaotic
