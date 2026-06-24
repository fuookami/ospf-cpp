#pragma once
/// Dyadic 混沌系统 / Dyadic chaotic system
/// 1:1 对应 Rust chaotic/dyadic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct DyadicParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kDyadicName[] = "dyadic";
    using DyadicSystem = ChaoticSystem2D<DyadicParams, kDyadicName>;
}
