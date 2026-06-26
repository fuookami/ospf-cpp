#pragma once
/// BPP3D domain/item/model/package_attribute/classification /// 1:1 对应 Rust bpp3d/domain/item/model/package_attribute/classification.rs
/// PackageClassification + PackageCategory + PackageType

#include <array>
#include <string>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 包装分类 / Package classification
    // 对应 Rust PackageClassification
    // ============================================================================

    /// 包装分类 / Package classification
    /// 对应 Rust PackageClassification
    enum class PackageClassification {
        /// 外包装 / Outer package
        Outer,
        /// 内包装 / Inner package
        Inner
    };

    // ============================================================================
    // 包装类别 / Package category
    // 对应 Rust PackageCategory
    // ============================================================================

    /// 包装类别 / Package category
    /// 对应 Rust PackageCategory
    enum class PackageCategory {
        /// 硬箱 / Hard box
        HardBox,
        /// 托盘 / Pallet
        Pallet,
        /// 软箱 / Soft box
        SoftBox,
        /// 填充物 / Filler
        Filler
    };

    // ============================================================================
    // 包装类型 / Package type
    // 对应 Rust PackageType
    // ============================================================================

    /// 包装类型 / Package type
    /// 对应 Rust PackageType
    enum class PackageType {
        /// 重型瓦楞纸托 / Duty corrugated board pedal
        DutyCorrugatedBoardPedal,
        /// 木箱 / Wooden container
        WoodenContainer,
        /// 蜂窝箱 / Honeycomb box
        HoneycombBox,
        /// 托盘 / Pallet
        Pallet,
        /// 纸箱托盘 / Carton pallet
        CartonPallet,
        /// 纸箱 / Carton container
        CartonContainer,
        /// 包装泡棉 / Packing foam
        PackingFoam
    };

    /// 所有包装类型 / All package types
    /// 对应 Rust PackageType::ALL
    inline constexpr std::array<PackageType, 7> kAllPackageTypes = {
        PackageType::DutyCorrugatedBoardPedal,
        PackageType::WoodenContainer,
        PackageType::HoneycombBox,
        PackageType::Pallet,
        PackageType::CartonPallet,
        PackageType::CartonContainer,
        PackageType::PackingFoam,
    };

    /// 包装类别 / Package category
    /// 对应 Rust PackageType::category
    [[nodiscard]] inline constexpr PackageCategory package_category(PackageType type) noexcept {
        switch (type) {
            case PackageType::DutyCorrugatedBoardPedal:
            case PackageType::WoodenContainer:
            case PackageType::HoneycombBox:
                return PackageCategory::HardBox;
            case PackageType::Pallet:
            case PackageType::CartonPallet:
                return PackageCategory::Pallet;
            case PackageType::CartonContainer:
                return PackageCategory::SoftBox;
            case PackageType::PackingFoam:
                return PackageCategory::Filler;
        }
        return PackageCategory::HardBox;
    }

    /// 默认包装类型 / Default package type
    /// 对应 Rust Default for PackageType
    inline constexpr PackageType kDefaultPackageType = PackageType::CartonContainer;

}  // namespace ospf::framework::bpp3d
