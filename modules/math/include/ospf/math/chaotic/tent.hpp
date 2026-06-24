#pragma once
/// Tent 混沌系统 / Tent chaotic system
/// 1:1 对应 Rust chaotic/tent.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct TentParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kTentName[] = "tent";
    using TentSystem = ChaoticSystem2D<TentParams, kTentName>;
}
