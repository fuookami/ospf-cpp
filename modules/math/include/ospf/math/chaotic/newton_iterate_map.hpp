#pragma once
/// NewtonIterateMap 标量映射 / NewtonIterate scalar map
/// 1:1 对应 Rust chaotic/newton_iterate.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct NewtonIterateMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kNewtonIterateMapName[] = "newton_iterate_map";
    using NewtonIterateMap = ScalarMap<NewtonIterateMapParams, kNewtonIterateMapName>;
}
