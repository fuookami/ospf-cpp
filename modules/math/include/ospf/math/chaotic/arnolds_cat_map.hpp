#pragma once
/// ArnoldsCatMap 混沌系统 / ArnoldsCatMap chaotic system
/// 1:1 对应 Rust chaotic/arnolds_cat_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ArnoldsCatMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kArnoldsCatMapName[] = "arnolds_cat_map";
    using ArnoldsCatMapSystem = ChaoticSystem2D<ArnoldsCatMapParams, kArnoldsCatMapName>;
}
