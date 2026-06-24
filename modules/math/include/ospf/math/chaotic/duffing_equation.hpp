#pragma once
/// DuffingEquation 混沌系统 / DuffingEquation chaotic system
/// 1:1 对应 Rust chaotic/duffing_equation.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct DuffingEquationParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kDuffingEquationName[] = "duffing_equation";
    using DuffingEquationSystem = ChaoticSystem3D<DuffingEquationParams, kDuffingEquationName>;
}
