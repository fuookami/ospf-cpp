#pragma once
/// KickedRotator 混沌系统 / KickedRotator chaotic system
/// 1:1 对应 Rust chaotic/kicked_rotator.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct KickedRotatorParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kKickedRotatorName[] = "kicked_rotator";
    using KickedRotatorSystem = ChaoticSystem2D<KickedRotatorParams, kKickedRotatorName>;
}
