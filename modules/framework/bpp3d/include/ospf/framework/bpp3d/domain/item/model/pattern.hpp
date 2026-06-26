#pragma once
/// BPP3D domain/item/model/pattern /// 1:1 对应 Rust bpp3d/domain/item/model/pattern.rs
/// BottomDimensionRange + PatternConfig + PatternStep + PatternProjectionOrientation

#include <algorithm>
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 模式投影方向 / Pattern projection orientation
    // 对应 Rust PatternProjectionOrientation
    // ============================================================================

    /// 模式投影方向 / Pattern projection orientation
    /// 对应 Rust PatternProjectionOrientation
    enum class PatternProjectionOrientation {
        /// 前向，要求长度不小于宽度 / Front, requiring length no smaller than width
        Front,
        /// 侧向，要求长度不大于宽度 / Side, requiring length no greater than width
        Side
    };

    // ============================================================================
    // 模式下一点策略 / Pattern next-point policy
    // 对应 Rust PatternNextPointPolicy
    // ============================================================================

    /// 模式下一点策略 / Pattern next-point policy
    /// 对应 Rust PatternNextPointPolicy
    enum class PatternNextPointPolicy {
        /// 右下角追加 / Append at right-bottom
        RightBottom,
        /// 左上角追加 / Append at left-upper
        LeftUpper
    };

    // ============================================================================
    // 模式步骤 / Pattern step
    // 对应 Rust PatternStep
    // ============================================================================

    /// 模式步骤 / Pattern step
    /// 对应 Rust PatternStep
    struct PatternStep {
        /// 长度投影方向 / Length projection orientation
        PatternProjectionOrientation length_orientation = PatternProjectionOrientation::Front;
        /// 下一点策略 / Next-point policy
        std::optional<PatternNextPointPolicy> next_point_policy;

        /// 创建模式步骤 / Create pattern step
        [[nodiscard]] static PatternStep create(
            PatternProjectionOrientation orientation,
            std::optional<PatternNextPointPolicy> policy = std::nullopt)
        {
            return PatternStep{orientation, policy};
        }
    };

    // ============================================================================
    // 底面尺寸范围 / Bottom dimension range
    // 对应 Rust BottomDimensionRange
    // ============================================================================

    /// 底面尺寸范围 / Bottom dimension range
    /// 对应 Rust BottomDimensionRange
    /// 按货物底面尺寸过滤模式候选
    struct BottomDimensionRange {
        /// 下界（含）/ Lower bound (inclusive)
        std::optional<double> min;
        /// 上界（含）/ Upper bound (inclusive)
        std::optional<double> max;

        /// 创建无约束范围 / Create unbounded range
        /// 对应 Rust BottomDimensionRange::unbounded
        [[nodiscard]] static BottomDimensionRange unbounded() {
            return BottomDimensionRange{std::nullopt, std::nullopt};
        }

        /// 创建有界范围 / Create bounded range
        /// 对应 Rust BottomDimensionRange::new
        [[nodiscard]] static BottomDimensionRange create(
            std::optional<double> min_val,
            std::optional<double> max_val)
        {
            return BottomDimensionRange{min_val, max_val};
        }

        /// 创建仅下界范围 / Create lower-bounded range
        /// 对应 Rust BottomDimensionRange::at_least
        [[nodiscard]] static BottomDimensionRange at_least(double min_val) {
            return BottomDimensionRange{min_val, std::nullopt};
        }

        /// 创建仅上界范围 / Create upper-bounded range
        /// 对应 Rust BottomDimensionRange::at_most
        [[nodiscard]] static BottomDimensionRange at_most(double max_val) {
            return BottomDimensionRange{std::nullopt, max_val};
        }

        /// 创建闭区间范围 / Create closed-interval range
        /// 对应 Rust BottomDimensionRange::between
        [[nodiscard]] static BottomDimensionRange between(double min_val, double max_val) {
            return BottomDimensionRange{min_val, max_val};
        }

        /// 判断是否无约束 / Check whether unbounded
        /// 对应 Rust BottomDimensionRange::is_unbounded
        [[nodiscard]] bool is_unbounded() const noexcept {
            return !min.has_value() && !max.has_value();
        }

        /// 判断值是否在范围内 / Check whether value is within range
        /// 对应 Rust BottomDimensionRange::contains
        [[nodiscard]] bool contains(double value) const noexcept {
            if (min.has_value() && value < *min) return false;
            if (max.has_value() && value > *max) return false;
            return true;
        }

        [[nodiscard]] bool operator==(const BottomDimensionRange& other) const noexcept {
            return min == other.min && max == other.max;
        }
    };

    // ============================================================================
    // 模式配置 / Pattern config
    // 对应 Rust PatternConfig
    // ============================================================================

    /// 模式配置 / Pattern config
    /// 对应 Rust PatternConfig
    struct PatternConfig {
        /// 允许堆叠层数 / Allowed piling layer count
        std::optional<std::size_t> with_piling;
        /// 是否允许余项模式 / Whether remainder patterns are allowed
        bool with_remainder = false;
        /// 模式列表 / Pattern list
        std::vector<std::vector<PatternStep>> patterns;
        /// 底面长度范围（底面较长维度）/ Bottom length range (longer bottom dimension)
        BottomDimensionRange bottom_length_range = BottomDimensionRange::unbounded();
        /// 底面宽度范围（底面较短维度）/ Bottom width range (shorter bottom dimension)
        BottomDimensionRange bottom_width_range = BottomDimensionRange::unbounded();

        /// 创建配置 / Create config
        [[nodiscard]] static PatternConfig create() {
            return PatternConfig{};
        }

        /// 设置底面长度范围 / Set bottom length range
        [[nodiscard]] PatternConfig with_bottom_length_range(BottomDimensionRange range) const {
            PatternConfig copy = *this;
            copy.bottom_length_range = std::move(range);
            return copy;
        }

        /// 设置底面宽度范围 / Set bottom width range
        [[nodiscard]] PatternConfig with_bottom_width_range(BottomDimensionRange range) const {
            PatternConfig copy = *this;
            copy.bottom_width_range = std::move(range);
            return copy;
        }

        /// 判断底面尺寸是否可接受 / Check whether bottom dimensions are acceptable
        /// 对应 Rust PatternConfig::accepts_bottom_dimensions
        /// bottom_length = max(depth, width), bottom_width = min(depth, width)
        [[nodiscard]] bool accepts_bottom_dimensions(double depth, double width) const {
            double bottom_length = std::max(depth, width);
            double bottom_width = std::min(depth, width);
            return bottom_length_range.contains(bottom_length)
                && bottom_width_range.contains(bottom_width);
        }
    };

}  // namespace ospf::framework::bpp3d
