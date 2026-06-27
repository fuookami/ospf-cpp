#pragma once
/// YuWang 混沌系统 / YuWang chaotic system
/// 1:1 对应 Rust chaotic/yu_wang.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct YuWangParams {
        double a = 10.0;
        double b = 40.0;
        double c = 2.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x), S{b}*x - S{c}*y - x*z, x*y - S{8.0/3.0}*z};
        }
    };

    static constexpr char kYuWangName[] = "yu_wang";
    using YuWangSystem = ChaoticSystem3D<YuWangParams, kYuWangName>;

}  // namespace ospf::math::chaotic
