#pragma once
/// ArnoldsCatMapMap 标量映射 / ArnoldsCatMap scalar map
/// 1:1 对应 Rust chaotic/arnolds_cat_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ArnoldsCatMapMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kArnoldsCatMapMapName[] = "arnolds_cat_map_map";
    using ArnoldsCatMapMap = ScalarMap<ArnoldsCatMapMapParams, kArnoldsCatMapMapName>;
}
