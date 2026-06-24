#pragma once
/// LiuChen 混沌系统 / LiuChen chaotic system
/// 1:1 对应 Rust chaotic/liu_chen.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct LiuChenParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kLiuChenName[] = "liu_chen";
    using LiuChenSystem = ChaoticSystem3D<LiuChenParams, kLiuChenName>;
}
