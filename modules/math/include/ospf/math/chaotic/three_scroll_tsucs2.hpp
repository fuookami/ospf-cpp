#pragma once
/// ThreeScrollTsucs2 混沌系统 / ThreeScrollTsucs2 chaotic system
/// 1:1 对应 Rust chaotic/three_scroll_tsucs2.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ThreeScrollTsucs2Params {
        double a = 40.0;
        double b = 0.833;
        double c = 0.5;
        double d = 0.65;
        double e = 20.0;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {S{a}*(y - x) + S{c}*x*z, S{e}*x - y - x*z, S{b}*z + x*y - S{d}*x*x};
        }
    };

    static constexpr char kThreeScrollTsucs2Name[] = "three_scroll_tsucs2";
    using ThreeScrollTsucs2System = ChaoticSystem3D<ThreeScrollTsucs2Params, kThreeScrollTsucs2Name>;

}  // namespace ospf::math::chaotic
