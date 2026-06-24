#pragma once
/// SineMap 混沌系统 / SineMap chaotic system
/// 1:1 对应 Rust chaotic/sine_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct SineMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kSineMapName[] = "sine_map";
    using SineMapSystem = ChaoticSystem2D<SineMapParams, kSineMapName>;
}
