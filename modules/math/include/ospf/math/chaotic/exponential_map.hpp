#pragma once
/// ExponentialMap 标量映射 / Exponential scalar map
/// 1:1 对应 Rust chaotic/exponential.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ExponentialMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kExponentialMapName[] = "exponential_map";
    using ExponentialMap = ScalarMap<ExponentialMapParams, kExponentialMapName>;
}
