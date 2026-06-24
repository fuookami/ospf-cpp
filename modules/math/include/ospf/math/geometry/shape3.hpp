#pragma once
/// 3D 形状 / 3D shape /// 1:1 对应 Rust geometry/shape3.rs
namespace ospf::math::geometry {
    template<typename S = double>
    struct Shape3 {
        S width;
        S height;
        S depth;
        [[nodiscard]] S volume() const { return width * height * depth; }
        [[nodiscard]] S surface_area() const {
            return S{2} * (width * height + height * depth + width * depth);
        }
        [[nodiscard]] bool operator==(const Shape3&) const = default;
    };
}
