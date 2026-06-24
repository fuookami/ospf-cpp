#pragma once
/// Lorenz96 混沌系统 / Lorenz96 chaotic system
/// 1:1 对应 Rust chaotic/lorenz96.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct Lorenz96Params {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kLorenz96Name[] = "lorenz96";
    using Lorenz96System = ChaoticSystem3D<Lorenz96Params, kLorenz96Name>;
}
