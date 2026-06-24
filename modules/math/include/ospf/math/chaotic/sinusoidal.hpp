#pragma once
/// Sinusoidal 混沌系统 / Sinusoidal chaotic system
/// 1:1 对应 Rust chaotic/sinusoidal.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct SinusoidalParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kSinusoidalName[] = "sinusoidal";
    using SinusoidalSystem = ChaoticSystem3D<SinusoidalParams, kSinusoidalName>;
}
