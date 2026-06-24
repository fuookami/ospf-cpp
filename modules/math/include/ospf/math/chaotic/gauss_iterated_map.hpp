#pragma once
/// GaussIteratedMap 标量映射 / GaussIterated scalar map
/// 1:1 对应 Rust chaotic/gauss_iterated.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct GaussIteratedMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kGaussIteratedMapName[] = "gauss_iterated_map";
    using GaussIteratedMap = ScalarMap<GaussIteratedMapParams, kGaussIteratedMapName>;
}
