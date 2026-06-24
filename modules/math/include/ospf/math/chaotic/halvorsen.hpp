#pragma once
/// Halvorsen 混沌系统 / Halvorsen chaotic system
/// 1:1 对应 Rust chaotic/halvorsen.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct HalvorsenParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kHalvorsenName[] = "halvorsen";
    using HalvorsenSystem = ChaoticSystem3D<HalvorsenParams, kHalvorsenName>;
}
