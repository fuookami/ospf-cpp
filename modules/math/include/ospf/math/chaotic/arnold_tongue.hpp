#pragma once
/// ArnoldTongue 混沌系统 / ArnoldTongue chaotic system
/// 1:1 对应 Rust chaotic/arnold_tongue.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct ArnoldTongueParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kArnoldTongueName[] = "arnold_tongue";
    using ArnoldTongueSystem = ChaoticSystem3D<ArnoldTongueParams, kArnoldTongueName>;
}
