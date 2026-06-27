#pragma once
/// Thomas 混沌系统 / Thomas chaotic system
/// 1:1 对应 Rust chaotic/thomas.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ThomasParams {
        double b = 0.21;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {std::sin(y) - S{b}*x, std::sin(z) - S{b}*y, std::sin(x) - S{b}*z};
        }
    };

    static constexpr char kThomasName[] = "thomas";
    using ThomasSystem = ChaoticSystem3D<ThomasParams, kThomasName>;

}  // namespace ospf::math::chaotic
