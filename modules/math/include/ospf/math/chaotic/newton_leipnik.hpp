#pragma once
/// NewtonLeipnik 混沌系统 / NewtonLeipnik chaotic system
/// 1:1 对应 Rust chaotic/newton_leipnik.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct NewtonLeipnikParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kNewtonLeipnikName[] = "newton_leipnik";
    using NewtonLeipnikSystem = ChaoticSystem3D<NewtonLeipnikParams, kNewtonLeipnikName>;
}
