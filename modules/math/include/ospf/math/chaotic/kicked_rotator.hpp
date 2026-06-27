#pragma once
/// KickedRotator 混沌系统 / KickedRotator chaotic system
/// 1:1 对应 Rust chaotic/kicked_rotator.rs
/// 2D 映射 / 2D map

#include <ospf/math/chaotic/chaotic_factory.hpp>

namespace ospf::math::chaotic {

    struct KickedRotatorParams {
        double k = 1.0;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            S dx = y;
            S dy = -x + S{k} * std::sin(y);
            return Point2<S>{dx, dy};
        }
    };

    static constexpr char kKickedRotatorName[] = "kicked_rotator";
    using KickedRotatorSystem = ChaoticSystem2D<KickedRotatorParams, kKickedRotatorName>;

}  // namespace ospf::math::chaotic
