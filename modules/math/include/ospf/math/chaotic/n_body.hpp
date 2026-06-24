#pragma once
/// NBody 混沌系统 / NBody chaotic system
/// 1:1 对应 Rust chaotic/n_body.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct NBodyParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kNBodyName[] = "n_body";
    using NBodySystem = ChaoticSystem3D<NBodyParams, kNBodyName>;
}
