#pragma once
/// ComplexSquaringMap 标量映射 / ComplexSquaring scalar map
/// 1:1 对应 Rust chaotic/complex_squaring.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ComplexSquaringMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kComplexSquaringMapName[] = "complex_squaring_map";
    using ComplexSquaringMap = ScalarMap<ComplexSquaringMapParams, kComplexSquaringMapName>;
}
