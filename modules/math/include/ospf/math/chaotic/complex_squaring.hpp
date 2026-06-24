#pragma once
/// ComplexSquaring 混沌系统 / ComplexSquaring chaotic system
/// 1:1 对应 Rust chaotic/complex_squaring.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ComplexSquaringParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kComplexSquaringName[] = "complex_squaring";
    using ComplexSquaringSystem = ChaoticSystem3D<ComplexSquaringParams, kComplexSquaringName>;
}
