#pragma once
/// SinusMap 混沌系统 / SinusMap chaotic system
/// 1:1 对应 Rust chaotic/sinus_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct SinusMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kSinusMapName[] = "sinus_map";
    using SinusMapSystem = ChaoticSystem2D<SinusMapParams, kSinusMapName>;
}
