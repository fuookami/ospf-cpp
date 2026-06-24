#pragma once
/// TentMap 标量映射 / Tent scalar map
/// 1:1 对应 Rust chaotic/tent.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct TentMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kTentMapName[] = "tent_map";
    using TentMap = ScalarMap<TentMapParams, kTentMapName>;
}
