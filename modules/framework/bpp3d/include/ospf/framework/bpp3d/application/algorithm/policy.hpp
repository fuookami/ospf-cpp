#pragma once
/// BPP3D application algorithm/policy
/// 1:1 对应 Rust bpp3d domain policy 类型
/// 包含深度边界策略、堆叠策略、悬空策略等

#include <string>
#include <vector>
#include <optional>
#include <unordered_set>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 深度边界策略 / Depth boundary policy
    // 对应 Rust CsvDepthBoundaryPolicy
    // ============================================================================

    /// 深度边界策略 / Depth boundary policy
    /// 控制首末层的允许方向和放置方式
    struct DepthBoundaryPolicy {
        /// 首层允许的圆柱轴方向 / Allowed cylinder axes for first layer
        std::unordered_set<std::string> first_layer_allowed_cylinder_axes;

        /// 末层允许的圆柱轴方向 / Allowed cylinder axes for last layer
        std::unordered_set<std::string> last_layer_allowed_cylinder_axes;

        /// 首层允许的长方体朝向 / Allowed cuboid orientations for first layer
        std::unordered_set<std::string> first_layer_allowed_cuboid_orientations;

        /// 末层允许的长方体朝向 / Allowed cuboid orientations for last layer
        std::unordered_set<std::string> last_layer_allowed_cuboid_orientations;

        /// 是否为空 / Is empty
        [[nodiscard]] bool is_empty() const {
            return first_layer_allowed_cylinder_axes.empty()
                && last_layer_allowed_cylinder_axes.empty()
                && first_layer_allowed_cuboid_orientations.empty()
                && last_layer_allowed_cuboid_orientations.empty();
        }
    };

    // ============================================================================
    // 悬空策略 / Hanging policy
    // 对应 Rust HangingPolicy
    // ============================================================================

    /// 悬空策略 / Hanging policy
    /// 控制物品是否允许悬空放置
    enum class HangingPolicy : std::uint8_t {
        Forbidden,    ///< 禁止悬空 / Hanging forbidden
        Allowed,      ///< 允许悬空 / Hanging allowed
        Conditional,  ///< 条件悬空 / Conditional hanging
    };

    /// 悬空策略名称 / Hanging policy name
    [[nodiscard]] inline const char* hanging_policy_name(HangingPolicy policy) noexcept {
        switch (policy) {
            case HangingPolicy::Forbidden: return "forbidden";
            case HangingPolicy::Allowed: return "allowed";
            case HangingPolicy::Conditional: return "conditional";
        }
        return "unknown";
    }

    // ============================================================================
    // 堆叠策略 / Stacking-on policy
    // 对应 Rust StackingOnPolicy
    // ============================================================================

    /// 堆叠策略 / Stacking-on policy
    /// 控制物品堆叠规则
    enum class StackingOnPolicy : std::uint8_t {
        Forbidden,         ///< 禁止堆叠 / Stacking forbidden
        SameCategoryOnly,  ///< 仅同类别堆叠 / Stack same category only
        Allowed,           ///< 允许堆叠 / Stacking allowed
    };

    /// 堆叠策略名称 / Stacking-on policy name
    [[nodiscard]] inline const char* stacking_on_policy_name(StackingOnPolicy policy) noexcept {
        switch (policy) {
            case StackingOnPolicy::Forbidden: return "forbidden";
            case StackingOnPolicy::SameCategoryOnly: return "same_category_only";
            case StackingOnPolicy::Allowed: return "allowed";
        }
        return "unknown";
    }

    // ============================================================================
    // 连续半径目标策略 / Continuous radius objective policy
    // 对应 Rust ContinuousRadiusObjectivePolicy
    // ============================================================================

    /// 连续半径目标策略 / Continuous radius objective policy
    /// 控制连续半径变量的目标函数权重
    enum class ContinuousRadiusObjectivePolicyType : std::uint8_t {
        None,                      ///< 无目标 / No objective
        MinimizeRadiusSquared,     ///< 最小化半径平方 / Minimize radius squared
    };

    /// 连续半径目标策略 / Continuous radius objective policy
    struct ContinuousRadiusObjectivePolicy {
        ContinuousRadiusObjectivePolicyType type = ContinuousRadiusObjectivePolicyType::None;
        double weight = 1.0;

        /// 策略名称 / Policy name
        [[nodiscard]] const char* name() const noexcept {
            switch (type) {
                case ContinuousRadiusObjectivePolicyType::None: return "none";
                case ContinuousRadiusObjectivePolicyType::MinimizeRadiusSquared: return "minimize_radius_squared";
            }
            return "unknown";
        }
    };

    // ============================================================================
    // 图案投影朝向 / Pattern projection orientation
    // 对应 Rust PatternProjectionOrientation
    // ============================================================================

    /// 图案投影朝向 / Pattern projection orientation
    enum class PatternProjectionOrientation : std::uint8_t {
        Horizontal,  ///< 水平投影 / Horizontal projection
        Vertical,    ///< 垂直投影 / Vertical projection
    };

    // ============================================================================
    // 图案下一步策略 / Pattern next point policy
    // 对应 Rust PatternNextPointPolicy
    // ============================================================================

    /// 图案下一步策略 / Pattern next point policy
    enum class PatternNextPointPolicy : std::uint8_t {
        Greedy,      ///< 贪心 / Greedy
        Optimal,     ///< 最优 / Optimal
    };

}  // namespace ospf::framework::bpp3d
