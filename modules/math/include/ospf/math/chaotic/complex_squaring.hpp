#pragma once
/// ComplexSquaring 混沌系统 / ComplexSquaring chaotic system
/// 1:1 对应 Rust chaotic/complex_squaring.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ComplexSquaringParams {
        double a = 0.0;
        double b = 0.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {x*x - y*y, S{2}*x*y};
        }
    };

    static constexpr char kComplexSquaringName[] = "complex_squaring";
    using ComplexSquaringSystem = ChaoticSystem2D<ComplexSquaringParams, kComplexSquaringName>;

}  // namespace ospf::math::chaotic
