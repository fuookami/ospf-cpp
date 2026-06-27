#pragma once
/// ThomasCyclicallySymmetric 混沌系统 / ThomasCyclicallySymmetric chaotic system
/// 1:1 对应 Rust chaotic/thomas_cyclically_symmetric.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ThomasCyclicallySymmetricParams {
        double b = 0.21;

        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            S x = state[0], y = state[1], z = state[2];
            return {std::sin(y) - S{b}*x, std::sin(z) - S{b}*y, std::sin(x) - S{b}*z};
        }
    };

    static constexpr char kThomasCyclicallySymmetricName[] = "thomas_cyclically_symmetric";
    using ThomasCyclicallySymmetricSystem = ChaoticSystem3D<ThomasCyclicallySymmetricParams, kThomasCyclicallySymmetricName>;

}  // namespace ospf::math::chaotic
