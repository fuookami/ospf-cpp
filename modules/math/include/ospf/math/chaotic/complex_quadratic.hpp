#pragma once
/// ComplexQuadratic 混沌系统 / ComplexQuadratic chaotic system
/// 1:1 对应 Rust chaotic/complex_quadratic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ComplexQuadraticParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kComplexQuadraticName[] = "complex_quadratic";
    using ComplexQuadraticSystem = ChaoticSystem3D<ComplexQuadraticParams, kComplexQuadraticName>;
}
