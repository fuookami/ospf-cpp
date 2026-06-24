#pragma once
/// Symplectic 混沌系统 / Symplectic chaotic system
/// 1:1 对应 Rust chaotic/symplectic.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct SymplecticParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kSymplecticName[] = "symplectic";
    using SymplecticSystem = ChaoticSystem3D<SymplecticParams, kSymplecticName>;
}
