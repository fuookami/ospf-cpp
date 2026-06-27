#pragma once
/// CoupledLorenz 混沌系统 / CoupledLorenz chaotic system
/// 1:1 对应 Rust chaotic/coupled_lorenz.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct CoupledLorenzParams {
        double sigma = 10.0;
        double rho = 28.0;
        double beta = 2.6666666666666665;
        double k = 0.1;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{sigma}*(y - x) + S{k}*(y - x), S{rho}*x - y - x*z, x*y - S{beta}*z};
        }
    };

    static constexpr char kCoupledLorenzName[] = "coupled_lorenz";
    using CoupledLorenzSystem = ChaoticSystem3D<CoupledLorenzParams, kCoupledLorenzName>;

}  // namespace ospf::math::chaotic
