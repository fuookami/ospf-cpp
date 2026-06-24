#pragma once
/// HindmarshRose 混沌系统 / HindmarshRose chaotic system
/// 1:1 对应 Rust chaotic/hindmarsh_rose.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct HindmarshRoseParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kHindmarshRoseName[] = "hindmarsh_rose";
    using HindmarshRoseSystem = ChaoticSystem3D<HindmarshRoseParams, kHindmarshRoseName>;
}
