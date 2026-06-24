#pragma once
/// 盒子形状 / Box shape /// 1:1 对应 Rust geometry/box_shape.rs
#include <algorithm>
namespace ospf::math::geometry {
    template<typename S = double>
    struct BoxShape {
        S width;
        S height;
        [[nodiscard]] S area() const { return width * height; }
        [[nodiscard]] S perimeter() const { return S{2} * (width + height); }
        [[nodiscard]] bool contains(S w, S h) const { return w <= width && h <= height; }
        [[nodiscard]] bool operator==(const BoxShape&) const = default;
    };
}
