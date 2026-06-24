#pragma once
/// Henon 混沌系统 / Henon chaotic system
/// 1:1 对应 Rust chaotic/henon.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct HenonParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kHenonName[] = "henon";
    using HenonSystem = ChaoticSystem2D<HenonParams, kHenonName>;
}
