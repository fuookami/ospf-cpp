#pragma once
/// BPP3D domain/item/model/package_attribute/physical_attributes /// 1:1 对应 Rust bpp3d/domain/item/model/package_attribute/physical_attributes.rs
/// WeightAttribute + DeformationAttribute + HangingPolicy + CargoAttributeKey

#include <optional>
#include <string>
#include <variant>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 货物属性键 / Cargo attribute key
    // 对应 Rust CargoAttributeKey
    // ============================================================================

    /// 货物属性键 / Cargo attribute key
    /// 对应 Rust CargoAttributeKey（被动元数据标签）
    struct CargoAttributeKey {
        /// 编号 / Number
        std::string no;
        /// 名称 / Name
        std::string name;

        [[nodiscard]] bool operator==(const CargoAttributeKey& other) const noexcept {
            return no == other.no && name == other.name;
        }

        [[nodiscard]] bool operator!=(const CargoAttributeKey& other) const noexcept {
            return !(*this == other);
        }

        [[nodiscard]] bool operator<(const CargoAttributeKey& other) const noexcept {
            return no < other.no || (no == other.no && name < other.name);
        }
    };

    // ============================================================================
    // 重量属性 / Weight attribute
    // 对应 Rust WeightAttribute
    // ============================================================================

    /// 重量属性 / Weight attribute
    /// 对应 Rust WeightAttribute
    struct WeightAttribute {
        /// 最大层数 / Maximum layer count
        std::optional<std::size_t> max_layer;
    };

    // ============================================================================
    // 变形属性 / Deformation attribute
    // 对应 Rust DeformationAttribute
    // ============================================================================

    /// 变形属性 / Deformation attribute
    /// 对应 Rust DeformationAttribute
    struct DeformationAttribute {
        /// 是否允许变形 / Whether deformation is allowed
        bool allow_deformation = false;
    };

    // ============================================================================
    // 悬空策略 / Hanging policy
    // 对应 Rust HangingPolicy（enum with Absolute/Relative variants）
    // ============================================================================

    /// 绝对悬空距离策略 / Absolute hanging-difference policy
    struct HangingPolicyAbsolute {
        /// 最大差值 / Maximum difference
        double max_difference = 0.0;
        /// 是否检查重量 / Whether to check weight
        bool with_weight = false;
    };

    /// 相对悬空面积比例策略 / Relative hanging-area policy
    struct HangingPolicyRelative {
        /// 允许悬空比例 / Allowed hanging percentage
        double hanging_percentage = 0.0;
        /// 是否检查重量 / Whether to check weight
        bool with_weight = false;
    };

    /// 悬空策略 / Hanging policy
    /// 对应 Rust HangingPolicy（Absolute / Relative 变体）
    using HangingPolicy = std::variant<HangingPolicyAbsolute, HangingPolicyRelative>;

    /// 创建默认悬空策略 / Create default hanging policy
    /// 对应 Rust Default for HangingPolicy
    [[nodiscard]] inline HangingPolicy default_hanging_policy() {
        return HangingPolicyAbsolute{0.0, false};
    }

    /// 判断是否启用悬空支撑检查 / Check whether hanging support stacking is enabled
    /// 对应 Rust HangingPolicy::enabled_stacking_on_support
    /// Rust 参数：(item_weight, footprint_area, footprint_min_span, bottom_support_area, bottom_support_weight)
    /// hanging_area = (footprint_area - bottom_support_area).max(0.0)
    /// Absolute: if with_weight && bottom_support_weight < item_weight → false; hanging_area <= max_difference * footprint_min_span
    /// Relative: if with_weight && bottom_support_weight < item_weight → false; hanging_area / footprint_area <= hanging_percentage
    [[nodiscard]] inline bool hanging_policy_enabled_stacking_on_support(
        const HangingPolicy& policy,
        double item_weight,
        double footprint_area,
        double footprint_min_span,
        double bottom_support_area,
        double bottom_support_weight)
    {
        double hanging_area = std::max(footprint_area - bottom_support_area, 0.0);
        return std::visit([&](const auto& p) -> bool {
            using T = std::decay_t<decltype(p)>;
            if constexpr (std::is_same_v<T, HangingPolicyAbsolute>) {
                if (p.with_weight && bottom_support_weight < item_weight) return false;
                return hanging_area <= p.max_difference * footprint_min_span;
            } else if constexpr (std::is_same_v<T, HangingPolicyRelative>) {
                if (p.with_weight && bottom_support_weight < item_weight) return false;
                return hanging_area / footprint_area <= p.hanging_percentage;
            }
            return false;
        }, policy);
    }

}  // namespace ospf::framework::bpp3d
