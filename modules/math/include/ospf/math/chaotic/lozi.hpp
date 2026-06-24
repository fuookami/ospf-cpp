#pragma once
/// Lozi 混沌系统 / Lozi chaotic system
/// 1:1 对应 Rust chaotic/lozi.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct LoziParams {
        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            return {S{0}, S{0}};
        }
    };
    static constexpr char kLoziName[] = "lozi";
    using LoziSystem = ChaoticSystem2D<LoziParams, kLoziName>;
}
