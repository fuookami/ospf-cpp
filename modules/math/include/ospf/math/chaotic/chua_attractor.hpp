#pragma once
/// ChuaAttractor 混沌系统 / ChuaAttractor chaotic system
/// 1:1 对应 Rust chaotic/chua_attractor.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ChuaAttractorParams {
        double a = 15.6;
        double b = 28.0;
        double m0 = -1.143;
        double m1 = -0.714;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            S f = S{m1}*x + S{0.5}*(S{m0}-S{m1})*(std::abs(x+S{1})-std::abs(x-S{1}));
            return {S{a}*(y - x - f), S{b}*(x - y) + z, -S{1.0}*y};
        }
    };

    static constexpr char kChuaAttractorName[] = "chua_attractor";
    using ChuaAttractorSystem = ChaoticSystem3D<ChuaAttractorParams, kChuaAttractorName>;

}  // namespace ospf::math::chaotic
