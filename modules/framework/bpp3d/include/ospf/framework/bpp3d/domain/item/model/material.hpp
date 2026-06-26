#pragma once
/// BPP3D domain/item/model/material /// 1:1 对应 Rust bpp3d/domain/item/model/material.rs
/// MaterialType + MaterialKey + Material

#include <ospf/framework/bpp3d/domain/item/model/package_attribute/physical_attributes.hpp>

#include <optional>
#include <string>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 物料类型 / Material type
    // 对应 Rust MaterialType
    // ============================================================================

    /// 物料类型 / Material type
    /// 对应 Rust MaterialType
    enum class MaterialType {
        /// 原材料 / Raw material
        RawMaterial,
        /// 半成品 / Semi-finished product
        SemiFinishedProduct,
        /// 成品 / Finished product
        FinishedProduct
    };

    // ============================================================================
    // 物料标识 / Material key
    // 对应 Rust MaterialKey
    // ============================================================================

    /// 物料标识 / Material key
    /// 对应 Rust MaterialKey
    struct MaterialKey {
        /// 物料编号 / Material number
        std::string no;
        /// 物料类型 / Material type
        MaterialType material_type = MaterialType::RawMaterial;
        /// 制造商 / Manufacturer
        std::optional<std::string> manufacturer;
        /// 供应商 / Supplier
        std::optional<std::string> supplier;

        [[nodiscard]] bool operator==(const MaterialKey& other) const noexcept {
            return no == other.no && material_type == other.material_type
                && manufacturer == other.manufacturer && supplier == other.supplier;
        }
    };

    // ============================================================================
    // 物料 / Material
    // 对应 Rust Material<V, U>
    // ============================================================================

    /// 物料 / Material
    /// 对应 Rust Material<V, U>
    struct Material {
        /// 编号 / Number
        std::string no;
        /// 类型 / Type
        MaterialType material_type = MaterialType::RawMaterial;
        /// 名称 / Name
        std::string name;
        /// 制造商 / Manufacturer
        std::optional<std::string> manufacturer;
        /// 供应商 / Supplier
        std::optional<std::string> supplier;
        /// 仓库 / Warehouse
        std::optional<std::string> warehouse;
        /// 重量 / Weight
        double weight = 0.0;
        /// 货物属性键 / Cargo attribute key
        std::optional<CargoAttributeKey> cargo;

        /// 物料标识 / Material key
        /// 对应 Rust Material::key
        [[nodiscard]] MaterialKey key() const {
            return MaterialKey{no, material_type, manufacturer, supplier};
        }
    };

}  // namespace ospf::framework::bpp3d
