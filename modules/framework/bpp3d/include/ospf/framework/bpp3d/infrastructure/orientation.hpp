#pragma once
/// BPP3D infrastructure/orientation /// 1:1 对应 Rust bpp3d/infrastructure/orientation.rs
/// Orientation + OrientationCategory

#include <cstddef>

namespace ospf::framework::bpp3d {

    /// 朝向类别 / Orientation category
    /// 对应 Rust OrientationCategory
    enum class OrientationCategory {
        /// 直立 / Upright
        Upright,
        /// 侧放 / Side
        Side,
        /// 平放 / Lie
        Lie
    };

    /// 朝向 / Orientation
    /// 对应 Rust Orientation（6 个变体）
    enum class Orientation {
        /// 直立 / Upright (W-H-D → width=width, height=height, depth=depth)
        Upright,
        /// 直立旋转 / Upright rotated (W-H-D → width=depth, height=height, depth=width)
        UprightRotated,
        /// 侧放 / Side (W-H-D → width=height, height=width, depth=depth)
        Side,
        /// 侧放旋转 / Side rotated (W-H-D → width=depth, height=width, depth=height)
        SideRotated,
        /// 平放 / Lie (W-H-D → width=width, height=depth, depth=height)
        Lie,
        /// 平放旋转 / Lie rotated (W-H-D → width=height, height=depth, depth=width)
        LieRotated
    };

    /// 获取朝向类别 / Get orientation category
    /// 对应 Rust Orientation::category
    [[nodiscard]] inline OrientationCategory orientation_category(Orientation o) noexcept {
        switch (o) {
            case Orientation::Upright:
            case Orientation::UprightRotated:
                return OrientationCategory::Upright;
            case Orientation::Side:
            case Orientation::SideRotated:
                return OrientationCategory::Side;
            case Orientation::Lie:
            case Orientation::LieRotated:
                return OrientationCategory::Lie;
        }
        return OrientationCategory::Upright;
    }

    /// 是否为旋转朝向 / Whether rotated orientation
    [[nodiscard]] inline bool is_rotated(Orientation o) noexcept {
        return o == Orientation::UprightRotated
            || o == Orientation::SideRotated
            || o == Orientation::LieRotated;
    }

    /// 朝向数量 / Orientation count
    inline constexpr std::size_t kOrientationCount = 6;

    /// 全部朝向 / All orientations
    inline constexpr Orientation kAllOrientations[] = {
        Orientation::Upright,
        Orientation::UprightRotated,
        Orientation::Side,
        Orientation::SideRotated,
        Orientation::Lie,
        Orientation::LieRotated
    };

}  // namespace ospf::framework::bpp3d
