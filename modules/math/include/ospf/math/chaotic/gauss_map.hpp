#pragma once
/// GaussMap 混沌系统 / GaussMap chaotic system
/// 1:1 对应 Rust chaotic/gauss_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct GaussMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kGaussMapName[] = "gauss_map";
    using GaussMapSystem = ChaoticSystem2D<GaussMapParams, kGaussMapName>;
}
