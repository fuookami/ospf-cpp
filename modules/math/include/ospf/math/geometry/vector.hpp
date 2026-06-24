#pragma once
/// 几何向量 / Geometric vector /// 1:1 对应 Rust geometry/vector.rs
#include <cmath>
namespace ospf::math::geometry {
    template<typename S = double>
    struct GeoVector {
        S x;
        S y;
        [[nodiscard]] S magnitude() const { return std::sqrt(x * x + y * y); }
        [[nodiscard]] S dot(const GeoVector& o) const { return x * o.x + y * o.y; }
        [[nodiscard]] S cross(const GeoVector& o) const { return x * o.y - y * o.x; }
        [[nodiscard]] GeoVector operator+(const GeoVector& o) const { return {x + o.x, y + o.y}; }
        [[nodiscard]] GeoVector operator-(const GeoVector& o) const { return {x - o.x, y - o.y}; }
        [[nodiscard]] GeoVector operator*(S s) const { return {x * s, y * s}; }
        [[nodiscard]] bool operator==(const GeoVector&) const = default;
    };
}
