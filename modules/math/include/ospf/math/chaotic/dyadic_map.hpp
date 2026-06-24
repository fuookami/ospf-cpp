#pragma once
/// DyadicMap 标量映射 / Dyadic scalar map
/// 1:1 对应 Rust chaotic/dyadic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct DyadicMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kDyadicMapName[] = "dyadic_map";
    using DyadicMap = ScalarMap<DyadicMapParams, kDyadicMapName>;
}
