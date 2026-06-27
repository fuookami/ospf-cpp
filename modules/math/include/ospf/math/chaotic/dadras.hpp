#pragma once
/// Dadras 混沌系统 / Dadras chaotic system
/// 1:1 对应 Rust chaotic/dadras.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct DadrasParams {
        double a = 3.0;
        double b = 2.7;
        double c = 1.7;
        double d = 2.0;
        double e = 9.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {y - S{a}*x + S{b}*y*z, S{c}*y - x*z + z, S{d}*x*y - S{e}*z};
        }
    };

    static constexpr char kDadrasName[] = "dadras";
    using DadrasSystem = ChaoticSystem3D<DadrasParams, kDadrasName>;

}  // namespace ospf::math::chaotic
