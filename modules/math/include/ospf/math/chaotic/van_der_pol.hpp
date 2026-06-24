#pragma once
/// VanDerPol 混沌系统 / VanDerPol chaotic system
/// 1:1 对应 Rust chaotic/van_der_pol.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct VanDerPolParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kVanDerPolName[] = "van_der_pol";
    using VanDerPolSystem = ChaoticSystem3D<VanDerPolParams, kVanDerPolName>;
}
