#pragma once
/// Lorenz84 混沌系统 / Lorenz84 chaotic system
/// 1:1 对应 Rust chaotic/lorenz84.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct Lorenz84Params {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            // 简化实现：返回零导数（占位）
            // Simplified: returns zero derivatives (placeholder)
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kLorenz84Name[] = "lorenz84";
    using Lorenz84System = ChaoticSystem3D<Lorenz84Params, kLorenz84Name>;
}
