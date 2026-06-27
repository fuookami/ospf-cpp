#pragma once
/// Lozi 混沌系统 / Lozi chaotic system
/// 1:1 对应 Rust chaotic/lozi.rs

#include <ospf/math/chaotic/chaotic_factory.hpp>
#include <cmath>

namespace ospf::math::chaotic {

    struct LoziParams {
        double a = 1.7;
        double b = 0.5;

        template<typename S = double>
        [[nodiscard]] Point2<S> derivatives(const Point2<S>& state) const {
            S x = state[0], y = state[1];
            return {S{1} - S{a}*std::abs(x) + y, S{b}*x};
        }
    };

    static constexpr char kLoziName[] = "lozi";
    using LoziSystem = ChaoticSystem2D<LoziParams, kLoziName>;

}  // namespace ospf::math::chaotic
