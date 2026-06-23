#pragma once

/// 包围盒 / Bounding box

#include <ospf/math/geometry/point.hpp>
#include <algorithm>

namespace ospf::math::geometry {

    /// 轴对齐包围盒 / Axis-aligned bounding box
    template<typename T, std::size_t N>
    class BoundingBox {
    public:
        BoundingBox(Point<T, N> min, Point<T, N> max)
            : min_(std::move(min)), max_(std::move(max)) {}

        [[nodiscard]] static BoundingBox from_points(const Point<T, N>& a, const Point<T, N>& b) {
            Point<T, N> min_pt, max_pt;
            for (std::size_t i = 0; i < N; ++i) {
                min_pt[i] = std::min(a[i], b[i]);
                max_pt[i] = std::max(a[i], b[i]);
            }
            return BoundingBox(std::move(min_pt), std::move(max_pt));
        }

        [[nodiscard]] const Point<T, N>& min() const noexcept { return min_; }
        [[nodiscard]] const Point<T, N>& max() const noexcept { return max_; }

        /// 中心点 / Center
        [[nodiscard]] Point<T, N> center() const { return min_.midpoint(max_); }

        /// 点是否在包围盒内 / Check if point is inside
        [[nodiscard]] bool contains(const Point<T, N>& point) const {
            for (std::size_t i = 0; i < N; ++i) {
                if (point[i] < min_[i] || point[i] > max_[i]) return false;
            }
            return true;
        }

    private:
        Point<T, N> min_;
        Point<T, N> max_;
    };

    template<typename T>
    using BoundingBox2 = BoundingBox<T, 2>;

    using BoundingBox2d = BoundingBox2<double>;

}  // namespace ospf::math::geometry
