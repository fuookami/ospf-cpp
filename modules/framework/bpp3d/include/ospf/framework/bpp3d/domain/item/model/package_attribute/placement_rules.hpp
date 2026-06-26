#pragma once
/// BPP3D domain/item/model/package_attribute/placement_rules /// 1:1 对应 Rust bpp3d/domain/item/model/package_attribute/placement_rules.rs
/// StackingOnPolicy + PackagePlacementStackingRule + PackagePairStackingRule

#include <optional>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 堆叠策略 / Stacking-on policy
    // 对应 Rust StackingOnPolicy
    // ============================================================================

    /// 箱式堆叠策略 / Box stacking policy
    struct StackingOnPolicyBox {
        /// 最大差值 / Maximum difference
        double max_difference = 0.0;
        /// 最大超重 / Maximum over weight
        double max_over_weight = 0.0;
    };

    /// 托盘式堆叠策略 / Pallet stacking policy
    struct StackingOnPolicyPallet {
        /// 是否允许在托盘上堆叠 / Whether stacking on pallet is allowed
        bool allow_stacking = true;
    };

    /// 无限制堆叠策略 / Unlimited stacking policy
    struct StackingOnPolicyUnlimited {};

    /// 堆叠策略 / Stacking-on policy
    /// 对应 Rust StackingOnPolicy（Box / Pallet / Unlimited 变体）
    using StackingOnPolicy = std::variant<StackingOnPolicyBox, StackingOnPolicyPallet, StackingOnPolicyUnlimited>;

    /// 创建默认堆叠策略 / Create default stacking policy
    /// 对应 Rust Default for StackingOnPolicy
    [[nodiscard]] inline StackingOnPolicy default_stacking_on_policy() {
        return StackingOnPolicyUnlimited{};
    }

    // ============================================================================
    // 堆叠输入 / Stacking input
    // 对应 Rust PackageStackingInput
    // ============================================================================

    // forward declaration for bottom_item reference
    struct PackageAttribute;

    /// 堆叠输入 / Stacking input
    /// 对应 Rust PackageStackingInput
    struct PackageStackingInput {
        /// 项目宽度 / Item width
        double item_width = 0.0;
        /// 项目重量 / Item weight
        double item_weight = 0.0;
        /// 层 / Layer
        std::size_t layer = 0;
        /// 底部朝向 / Bottom orientation
        int bottom_orientation = 0;
        /// 底部朝向是否启用 / Whether bottom orientation is enabled
        bool bottom_orientation_enabled = false;
        /// 项目朝向 / Item orientation
        int item_orientation = 0;
        /// 项目朝向是否启用 / Whether item orientation is enabled
        bool item_orientation_enabled = false;
        /// 项目朝向是否在空间中启用 / Whether item orientation is enabled at space
        bool item_orientation_enabled_at_space = true;
        /// 空间宽度 / Space width
        double space_width = 0.0;
        /// 空间高度 / Space height
        double space_height = 0.0;
        /// 空间深度 / Space depth
        double space_depth = 0.0;
        /// 底部包装属性（非拥有指针） / Bottom item package attribute (non-owning pointer)
        const PackageAttribute* bottom_item = nullptr;
    };

    // ============================================================================
    // 放置堆叠规则 / Placement stacking rule
    // 对应 Rust PackagePlacementStackingRule
    // ============================================================================

    /// 放置堆叠规则 / Placement stacking rule
    /// 对应 Rust PackagePlacementStackingRule
    struct PackagePlacementStackingRule {
        /// 规则名称 / Rule name
        std::string name;
    };

    // ============================================================================
    // 配对堆叠规则 / Pair stacking rule
    // 对应 Rust PackagePairStackingRule
    // ============================================================================

    /// 配对堆叠规则 / Pair stacking rule
    /// 对应 Rust PackagePairStackingRule
    struct PackagePairStackingRule {
        /// 规则名称 / Rule name
        std::string name;
    };

}  // namespace ospf::framework::bpp3d
