#pragma once
/// NewtonIterate 混沌系统 / NewtonIterate chaotic system
/// 1:1 对应 Rust chaotic/newton_iterate.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct NewtonIterateParams {
        double a = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {y, -x + S{a}*y*(S{1} - x*x)};
        }
    };

    static constexpr char kNewtonIterateName[] = "newton_iterate";
    using NewtonIterateSystem = ChaoticSystem2D<NewtonIterateParams, kNewtonIterateName>;

}  // namespace ospf::math::chaotic
