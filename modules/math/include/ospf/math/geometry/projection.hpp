#pragma once
/// 投影 / Projection /// 1:1 对应 Rust geometry/projection.rs
#include <ospf/math/geometry/point.hpp>
namespace ospf::math::geometry {
    template<typename S = double>
    [[nodiscard]] S project_scalar(S value, S min, S max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
}
