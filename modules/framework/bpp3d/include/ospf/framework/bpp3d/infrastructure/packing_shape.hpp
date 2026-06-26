#pragma once
/// BPP3D infrastructure/packing_shape /// 1:1 对应 Rust bpp3d/infrastructure/packing_shape.rs
/// Axis3 + PackingShapeType + PackageShapeSpec

#include <variant>
#include <optional>
#include <string>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 3D 轴 / Axis3
    // 对应 Rust math::geometry::Axis3
    // ============================================================================

    /// 3D 轴 / Axis3
    /// 对应 Rust math::geometry::Axis3
    enum class Axis3 {
        X, Y, Z
    };

    // ============================================================================
    // 包装形状类型 / Packing shape type
    // 对应 Rust PackingShapeType
    // ============================================================================

    /// 包装形状类型 / Packing shape type
    /// 对应 Rust PackingShapeType
    enum class PackingShapeType {
        Cuboid,
        Cylinder
    };

    // ============================================================================
    // 包装形状规格 / Package shape spec
    // 对应 Rust PackageShapeSpec<V, U>（Cuboid / Cylinder 变体）
    // ============================================================================

    /// 包装形状规格 / Package shape spec
    /// 对应 Rust PackageShapeSpec<V, U>（Cuboid / Cylinder 变体）
    struct PackageShapeSpecCuboid {};

    struct PackageShapeSpecCylinder {
        Axis3 axis = Axis3::Y;
        double radius = 0.0;
    };

    using PackageShapeSpec = std::variant<PackageShapeSpecCuboid, PackageShapeSpecCylinder>;

    /// 创建长方体形状规格 / Create cuboid shape spec
    [[nodiscard]] inline PackageShapeSpec cuboid_shape_spec() {
        return PackageShapeSpecCuboid{};
    }

    /// 创建圆柱形状规格 / Create cylinder shape spec
    [[nodiscard]] inline PackageShapeSpec cylinder_shape_spec(Axis3 axis, double radius) {
        return PackageShapeSpecCylinder{axis, radius};
    }

    /// 判断是否为长方体 / Check whether cuboid
    [[nodiscard]] inline bool is_cuboid(const PackageShapeSpec& spec) {
        return std::holds_alternative<PackageShapeSpecCuboid>(spec);
    }

    /// 判断是否为圆柱 / Check whether cylinder
    [[nodiscard]] inline bool is_cylinder(const PackageShapeSpec& spec) {
        return std::holds_alternative<PackageShapeSpecCylinder>(spec);
    }

}  // namespace ospf::framework::bpp3d
