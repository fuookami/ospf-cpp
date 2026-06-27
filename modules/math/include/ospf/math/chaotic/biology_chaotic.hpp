#pragma once
/// BiologyChaotic 混沌系统 / BiologyChaotic chaotic system
/// 1:1 对应 Rust chaotic/biology_chaotic.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct BiologyChaoticParams {
        double a = 0.4;
        double b = 0.3;
        double c = 0.5;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {x*(S{1} - x - S{a}*y), y*(S{b}*x - S{c}), S{0.1}*z};
        }
    };

    static constexpr char kBiologyChaoticName[] = "biology_chaotic";
    using BiologyChaoticSystem = ChaoticSystem3D<BiologyChaoticParams, kBiologyChaoticName>;

}  // namespace ospf::math::chaotic
