#pragma once
/// ChebyshevMap 混沌系统 / ChebyshevMap chaotic system
/// 1:1 对应 Rust chaotic/chebyshev_map.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ChebyshevMapParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kChebyshevMapName[] = "chebyshev_map";
    using ChebyshevMapSystem = ChaoticSystem2D<ChebyshevMapParams, kChebyshevMapName>;
}
