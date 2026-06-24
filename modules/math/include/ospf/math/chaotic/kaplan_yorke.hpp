#pragma once
/// KaplanYorke 混沌系统 / KaplanYorke chaotic system
/// 1:1 对应 Rust chaotic/kaplan_yorke.rs
#include <ospf/math/chaotic/chaotic_factory.hpp>
namespace ospf::math::chaotic {
    struct KaplanYorkeParams {
        template<typename S = double>
        [[nodiscard]] Point3<S> derivatives(const Point3<S>& state) const {
            return {S{0}, S{0}, S{0}};
        }
    };
    static constexpr char kKaplanYorkeName[] = "kaplan_yorke";
    using KaplanYorkeSystem = ChaoticSystem3D<KaplanYorkeParams, kKaplanYorkeName>;
}
