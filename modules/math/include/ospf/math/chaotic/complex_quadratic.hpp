#pragma once
/// ComplexQuadratic 混沌系统 / ComplexQuadratic chaotic system
/// 1:1 对应 Rust chaotic/complex_quadratic.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct ComplexQuadraticParams {
        double a = 0.5;
        double b = 0.5;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {x*x - y*y + S{a}, S{2}*x*y + S{b}};
        }
    };

    static constexpr char kComplexQuadraticName[] = "complex_quadratic";
    using ComplexQuadraticSystem = ChaoticSystem2D<ComplexQuadraticParams, kComplexQuadraticName>;

}  // namespace ospf::math::chaotic
