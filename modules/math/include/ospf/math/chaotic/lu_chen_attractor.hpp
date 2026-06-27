#pragma once
/// LuChenAttractor 混沌系统 / LuChenAttractor chaotic system
/// 1:1 对应 Rust chaotic/lu_chen_attractor.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LuChenAttractorParams {
        double a = 36.0;
        double b = 3.0;
        double c = 20.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), (S{c}-S{a})*x - x*z + S{c}*y, x*y - S{b}*z};
        }
    };

    static constexpr char kLuChenAttractorName[] = "lu_chen_attractor";
    using LuChenAttractorSystem = ChaoticSystem3D<LuChenAttractorParams, kLuChenAttractorName>;

}  // namespace ospf::math::chaotic
