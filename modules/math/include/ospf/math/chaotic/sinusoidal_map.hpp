#pragma once
/// SinusoidalMap 标量映射 / Sinusoidal scalar map
/// 1:1 对应 Rust chaotic/sinusoidal.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct SinusoidalMapParams {
        template<typename S = double>
        [[nodiscard]] S map(S x) const { return x; }
    };
    static constexpr char kSinusoidalMapName[] = "sinusoidal_map";
    using SinusoidalMap = ScalarMap<SinusoidalMapParams, kSinusoidalMapName>;
}
