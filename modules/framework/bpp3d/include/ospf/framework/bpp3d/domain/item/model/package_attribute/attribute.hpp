#pragma once
/// BPP3D domain/item/model/package_attribute/attribute /// 1:1 对应 Rust bpp3d/domain/item/model/package_attribute/attribute.rs
/// PackageAttribute + Orientation + PackageOrientationRule + orientation_rules

#include <ospf/framework/bpp3d/infrastructure/orientation.hpp>
#include <ospf/framework/bpp3d/domain/item/model/package_attribute/classification.hpp>
#include <ospf/framework/bpp3d/domain/item/model/package_attribute/physical_attributes.hpp>
#include <ospf/framework/bpp3d/domain/item/model/package_attribute/placement_rules.hpp>

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 朝向规则 / Orientation rules
    // 对应 Rust PackageOrientationRule
    // ============================================================================

    /// 禁止朝向类别 / Forbid orientation category
    struct OrientationRuleForbidCategory {
        OrientationCategory category;
    };

    /// 禁止旋转 / Forbid rotated
    struct OrientationRuleForbidRotated {};

    /// 要求最小空间高度 / Require min space height
    struct OrientationRuleRequireMinSpaceHeight {
        double min_height = 0.0;
    };

    /// 要求最小空间宽度 / Require min space width
    struct OrientationRuleRequireMinSpaceWidth {
        double min_width = 0.0;
    };

    /// 要求最小空间深度 / Require min space depth
    struct OrientationRuleRequireMinSpaceDepth {
        double min_depth = 0.0;
    };

    /// 朝向规则 / Orientation rules
    /// 对应 Rust PackageOrientationRule
    using PackageOrientationRule = std::variant<
        OrientationRuleForbidCategory,
        OrientationRuleForbidRotated,
        OrientationRuleRequireMinSpaceHeight,
        OrientationRuleRequireMinSpaceWidth,
        OrientationRuleRequireMinSpaceDepth
    >;

    // ============================================================================
    // 朝向规则输入 / Orientation rule input
    // 对应 Rust PackageOrientationRuleInput
    // ============================================================================

    /// 朝向规则输入 / Orientation rule input
    /// 对应 Rust PackageOrientationRuleInput
    struct PackageOrientationRuleInput {
        Orientation orientation = Orientation::Upright;
        double space_width = 0.0;
        double space_height = 0.0;
        double space_depth = 0.0;
    };

    // ============================================================================
    // 包装属性 / Package attribute
    // 对应 Rust PackageAttribute
    // ============================================================================

    /// 包装属性 / Package attribute
    /// 对应 Rust PackageAttribute
    struct PackageAttribute {
        /// 包装类型 / Package type
        PackageType package_type = kDefaultPackageType;
        /// 包装最大层数 / Package maximum layer count
        std::optional<std::size_t> package_max_layer;
        /// 最大堆叠高度 / Maximum stacking height
        std::optional<double> max_height;
        /// 最小深度 / Minimum depth
        double min_depth = 0.0;
        /// 最大深度 / Maximum depth
        std::optional<double> max_depth;
        /// 可承载的上方包装类型 / Package types allowed above
        std::vector<PackageType> over_package_types;
        /// 是否只能在底部 / Whether bottom-only
        bool bottom_only = false;
        /// 顶面是否平整 / Whether top surface is flat
        bool top_flat = true;
        /// 侧放可放置的最高层 / Highest layer allowing side orientation on top
        std::size_t side_on_top_layer = 0;
        /// 平放可放置的最高层 / Highest layer allowing lie orientation on top
        std::size_t lie_on_top_layer = 0;
        /// 货物属性键 / Cargo attribute key
        std::optional<CargoAttributeKey> cargo_attribute;
        /// 重量属性 / Weight attribute
        WeightAttribute weight_attribute;
        /// 变形属性 / Deformation attribute
        DeformationAttribute deformation_attribute;
        /// 悬空策略 / Hanging policy
        HangingPolicy hanging_policy = default_hanging_policy();
        /// 堆叠策略 / Stacking-on policy
        StackingOnPolicy stacking_on_policy = default_stacking_on_policy();
        /// 是否允许混装 / Whether mixed loading is allowed
        std::optional<bool> allow_mixed_loading;
        /// 最大堆叠层数 / Maximum stacking layers
        std::optional<std::size_t> max_stack_layers;
        /// 业务标签 / Business tags
        std::vector<std::string> tags;
        /// 货物属性标签 / Cargo attribute tags
        std::vector<std::string> cargo_tags;
        /// 额外朝向规则 / Extra orientation rules
        std::vector<PackageOrientationRule> extra_orientation_rules;
        /// 额外两两堆叠规则 / Extra pair stacking rules
        std::vector<PackagePairStackingRule> extra_pair_stacking_rules;
        /// 额外放置级堆叠规则 / Extra placement stacking rules
        std::vector<PackagePlacementStackingRule> extra_placement_stacking_rules;

        /// 包装类别 / Package category
        /// 对应 Rust PackageAttribute::package_category
        [[nodiscard]] PackageCategory package_category() const noexcept {
            return ::ospf::framework::bpp3d::package_category(package_type);
        }

        /// 是否启用侧放上层限制 / Whether side-on-top limit is enabled
        /// 对应 Rust PackageAttribute::enabled_side_on_top
        [[nodiscard]] bool enabled_side_on_top() const noexcept {
            return side_on_top_layer != 0;
        }

        /// 是否启用平放上层限制 / Whether lie-on-top limit is enabled
        /// 对应 Rust PackageAttribute::enabled_lie_on_top
        [[nodiscard]] bool enabled_lie_on_top() const noexcept {
            return lie_on_top_layer != 0;
        }

        /// 最大层数 / Max layer
        /// 对应 Rust PackageAttribute::max_layer
        [[nodiscard]] std::optional<std::size_t> max_layer() const noexcept {
            if (package_max_layer.has_value() && weight_attribute.max_layer.has_value()) {
                return std::min(*package_max_layer, *weight_attribute.max_layer);
            }
            if (package_max_layer.has_value()) return package_max_layer;
            if (weight_attribute.max_layer.has_value()) return weight_attribute.max_layer;
            return max_stack_layers;
        }

        /// 悬空支撑检查 / Hanging support stacking check
        /// 对应 Rust PackageAttribute::enabled_stacking_on_support
        [[nodiscard]] bool enabled_stacking_on_support(
            double item_weight,
            double footprint_area,
            double footprint_min_span,
            double bottom_support_area,
            double bottom_support_weight) const
        {
            return hanging_policy_enabled_stacking_on_support(
                hanging_policy,
                item_weight,
                footprint_area,
                footprint_min_span,
                bottom_support_area,
                bottom_support_weight);
        }

        /// 诊断信息 / Diagnostics
        /// 对应 Rust PackageAttribute::packing_diagnostics
        [[nodiscard]] std::vector<std::string> packing_diagnostics(std::size_t layer_count) const {
            std::vector<std::string> diagnostics;
            if (max_stack_layers.has_value() && layer_count > *max_stack_layers) {
                diagnostics.push_back("package attribute max_stack_layers exceeded: "
                    + std::to_string(layer_count) + " > " + std::to_string(*max_stack_layers));
            }
            if (allow_mixed_loading.has_value() && !*allow_mixed_loading) {
                diagnostics.push_back("package attribute disallows mixed loading");
            }
            auto ml = max_layer();
            if (ml.has_value() && layer_count > *ml) {
                diagnostics.push_back("package attribute max_layer exceeded: "
                    + std::to_string(layer_count) + " > " + std::to_string(*ml));
            }
            return diagnostics;
        }

        /// 判断朝向规则是否允许 / Check whether orientation is enabled by rules
        /// 对应 Rust PackageAttribute::enabled_orientation_by_rule
        [[nodiscard]] bool enabled_orientation_by_rule(const PackageOrientationRuleInput& input) const {
            for (const auto& rule : extra_orientation_rules) {
                if (!check_orientation_rule(rule, input)) return false;
            }
            return true;
        }

    private:
        [[nodiscard]] static bool check_orientation_rule(
            const PackageOrientationRule& rule,
            const PackageOrientationRuleInput& input)
        {
            return std::visit([&](const auto& r) -> bool {
                using T = std::decay_t<decltype(r)>;
                if constexpr (std::is_same_v<T, OrientationRuleForbidCategory>) {
                    return orientation_category(input.orientation) != r.category;
                } else if constexpr (std::is_same_v<T, OrientationRuleForbidRotated>) {
                    return input.orientation != Orientation::UprightRotated;
                } else if constexpr (std::is_same_v<T, OrientationRuleRequireMinSpaceHeight>) {
                    return input.space_height >= r.min_height;
                } else if constexpr (std::is_same_v<T, OrientationRuleRequireMinSpaceWidth>) {
                    return input.space_width >= r.min_width;
                } else if constexpr (std::is_same_v<T, OrientationRuleRequireMinSpaceDepth>) {
                    return input.space_depth >= r.min_depth;
                }
                return true;
            }, rule);
        }
    };

}  // namespace ospf::framework::bpp3d
