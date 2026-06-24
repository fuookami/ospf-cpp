#pragma once
/// ComplexQuadraticMap 标量映射 / ComplexQuadratic scalar map
/// 1:1 对应 Rust chaotic/complex_quadratic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ComplexQuadraticMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kComplexQuadraticMapName[] = "complex_quadratic_map";
    using ComplexQuadraticMap = ScalarMap<ComplexQuadraticMapParams, kComplexQuadraticMapName>;
}
