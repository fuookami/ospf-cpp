#pragma once
/// Finance 混沌系统 / Finance chaotic system
/// 1:1 对应 Rust chaotic/finance.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct FinanceParams {
        double a = 0.001;
        double b = 0.2;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {(S{1}/S{b} - S{a})*x + z + x*y, -y - x*x, -S{c}*x};
        }
    };

    static constexpr char kFinanceName[] = "finance";
    using FinanceSystem = ChaoticSystem3D<FinanceParams, kFinanceName>;

}  // namespace ospf::math::chaotic
