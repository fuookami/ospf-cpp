#pragma once
/// LuChenSystem 混沌系统 / LuChenSystem chaotic system
/// 1:1 对应 Rust chaotic/lu_chen_system.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LuChenSystemParams {
        double a = 40.0;
        double b = 2.0;
        double c = 55.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), (S{c}-S{a})*x - x*z + S{c}*y, x*y - S{b}*z};
        }
    };

    static constexpr char kLuChenSystemName[] = "lu_chen_system";
    using LuChenSystemSystem = ChaoticSystem3D<LuChenSystemParams, kLuChenSystemName>;

}  // namespace ospf::math::chaotic
