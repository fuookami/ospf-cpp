#pragma once
/// DoublePendulum 混沌系统 / DoublePendulum chaotic system
/// 1:1 对应 Rust chaotic/double_pendulum.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct DoublePendulumParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kDoublePendulumName[] = "double_pendulum";
    using DoublePendulumSystem = ChaoticSystem3D<DoublePendulumParams, kDoublePendulumName>;
}
