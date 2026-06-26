#pragma once
/// BPP3D domain/item/model/item /// 1:1 对应 Rust bpp3d/domain/item/model/item.rs
/// ActualItem + Package + PackageShape + ActualItemVolume

#include <ospf/framework/bpp3d/infrastructure/packing_shape.hpp>
#include <ospf/framework/bpp3d/infrastructure/orientation.hpp>

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 包装形状 / Package shape
    // 对应 Rust PackageShape<V, U>
    // ============================================================================

    /// 包装形状 / Package shape
    /// 对应 Rust PackageShape<V, U>
    struct PackageShape {
        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;
        double weight = 0.0;
        PackageShapeSpec spec = PackageShapeSpecCuboid{};
    };

    // ============================================================================
    // 包装 / Package
    // 对应 Rust Package<V, U>
    // ============================================================================

    /// 包装 / Package
    /// 对应 Rust Package<V, U>
    struct Package {
        /// 包装编码 / Package code
        std::optional<std::string> code;
        /// 形状 / Shape
        PackageShape shape;
        /// 物料清单 / Materials (material_key, amount)
        std::vector<std::pair<std::string, std::size_t>> materials;
        /// 包装数量 / Package amount
        std::size_t amount = 1;
    };

    // ============================================================================
    // 实际货物 / Actual item
    // 对应 Rust ActualItem<V, U>
    // ============================================================================

    /// 实际货物 / Actual item
    /// 对应 Rust ActualItem<V, U>
    struct ActualItem {
        /// 标识 / ID
        std::string id;
        /// 名称 / Name
        std::string name;
        /// 包装编码 / Package code
        std::optional<std::string> package_code;
        /// 包装 / Package
        std::optional<Package> pack;
        /// 宽度 / Width
        double width = 0.0;
        /// 高度 / Height
        double height = 0.0;
        /// 深度 / Depth
        double depth = 0.0;
        /// 重量 / Weight
        double weight = 0.0;
        /// 允许朝向 / Enabled orientations
        std::vector<Orientation> enabled_orientations;
        /// 形状规格覆盖 / Shape specification override
        std::optional<PackageShapeSpec> shape_spec_override;

        /// 体积 / Volume
        /// 对应 Rust ActualItem::volume
        [[nodiscard]] double volume() const noexcept {
            return width * height * depth;
        }

        /// 是否为长方体 / Whether cuboid
        /// 对应 Rust ActualItem::is_cuboid
        [[nodiscard]] bool is_cuboid() const noexcept {
            if (!shape_spec_override.has_value()) return true;
            return ospf::framework::bpp3d::is_cuboid(*shape_spec_override);
        }

        /// 包装形状 / Packing shape
        /// 对应 Rust ActualItem::packing_shape
        /// 返回货物在当前配置下的包装形状（Shape/PackingShapeType/Axis/bounding_width/height/depth）
        [[nodiscard]] auto packing_shape() const {
            struct PackingShape {
                PackingShapeType shape_type = PackingShapeType::Cuboid;
                std::optional<Axis3> axis;
                double bounding_width = 0.0;
                double bounding_height = 0.0;
                double bounding_depth = 0.0;
            };
            PackingShape ps;
            ps.bounding_width = width;
            ps.bounding_height = height;
            ps.bounding_depth = depth;

            if (shape_spec_override.has_value()) {
                if (std::holds_alternative<PackageShapeSpecCylinder>(*shape_spec_override)) {
                    ps.shape_type = PackingShapeType::Cylinder;
                    ps.axis = std::get<PackageShapeSpecCylinder>(*shape_spec_override).axis;
                } else {
                    ps.shape_type = PackingShapeType::Cuboid;
                }
            } else {
                ps.shape_type = PackingShapeType::Cuboid;
            }
            return ps;
        }

        /// 朝向包装形状 / Oriented packing shape
        /// 对应 Rust ActualItem::oriented_packing_shape
        /// 计算货物在指定朝向下的包装形状（宽高深交换，圆柱轴随朝向更新）
        [[nodiscard]] auto oriented_packing_shape(Orientation orient) const {
            auto ps = packing_shape();

            // 交换宽高深 / Swap dimensions by orientation
            switch (orient) {
                case Orientation::Upright:
                case Orientation::UprightRotated:
                    // 保持不变
                    break;
                case Orientation::Side:
                case Orientation::SideRotated: {
                    // width ↔ height
                    std::swap(ps.bounding_width, ps.bounding_height);
                    if (ps.axis.has_value()) {
                        if (*ps.axis == Axis3::Y) ps.axis = Axis3::X;
                        else if (*ps.axis == Axis3::X) ps.axis = Axis3::Y;
                    }
                    break;
                }
                case Orientation::Lie:
                case Orientation::LieRotated: {
                    // height ↔ depth
                    std::swap(ps.bounding_height, ps.bounding_depth);
                    if (ps.axis.has_value()) {
                        if (*ps.axis == Axis3::Y) ps.axis = Axis3::Z;
                        else if (*ps.axis == Axis3::Z) ps.axis = Axis3::Y;
                    }
                    break;
                }
            }
            return ps;
        }
    };

}  // namespace ospf::framework::bpp3d
