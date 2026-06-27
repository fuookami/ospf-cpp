#pragma once
/// DequanLi 混沌系统 / DequanLi chaotic system
/// 1:1 对应 Rust chaotic/dequan_li.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct DequanLiParams {
        double a = 40.0;
        double b = 1.833;
        double c = 0.16;
        double d = 0.65;
        double e = 55.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y-x) + S{c}*x*z, S{e}*x - y - x*z, S{b}*z + x*y - S{d}*x*x};
        }
    };

    static constexpr char kDequanLiName[] = "dequan_li";
    using DequanLiSystem = ChaoticSystem3D<DequanLiParams, kDequanLiName>;

}  // namespace ospf::math::chaotic
