#pragma once
/// BakersMap 混沌系统 / BakersMap chaotic system
/// 1:1 对应 Rust chaotic/bakers_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct BakersMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kBakersMapName[] = "bakers_map";
    using BakersMapSystem = ChaoticSystem2D<BakersMapParams, kBakersMapName>;
}
