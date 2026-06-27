#pragma once
/// Gingerbreadman 混沌系统 / Gingerbreadman chaotic system
/// 1:1 对应 Rust chaotic/gingerbreadman.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct GingerbreadmanParams {
        double a = 1.0;
        double b = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{1} - S{a}*y + std::abs(x), x};
        }
    };

    static constexpr char kGingerbreadmanName[] = "gingerbreadman";
    using GingerbreadmanSystem = ChaoticSystem2D<GingerbreadmanParams, kGingerbreadmanName>;

}  // namespace ospf::math::chaotic
