#pragma once
/// BogdanovMap 混沌系统 / BogdanovMap chaotic system
/// 1:1 对应 Rust chaotic/bogdanov_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct BogdanovMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kBogdanovMapName[] = "bogdanov_map";
    using BogdanovMapSystem = ChaoticSystem2D<BogdanovMapParams, kBogdanovMapName>;
}
