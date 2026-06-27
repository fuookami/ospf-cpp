#pragma once
/// Bouali 混沌系统 / Bouali chaotic system
/// 1:1 对应 Rust chaotic/bouali.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct BoualiParams {
        double a = 0.1;
        double b = 1.0;
        double c = 1.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {x*(S{4}-y) + S{a}*z, -y*(S{1}-x*x), -x - S{b}*y - S{c}*z};
        }
    };

    static constexpr char kBoualiName[] = "bouali";
    using BoualiSystem = ChaoticSystem3D<BoualiParams, kBoualiName>;

}  // namespace ospf::math::chaotic
