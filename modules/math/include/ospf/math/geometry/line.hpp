#pragma once

/// 线段和直线 / Line segment and line

#include <ospf/math/geometry/point.hpp>
#include <cmath>
#include <optional>

namespace ospf::math::geometry {

    /// 线段 / Line segment
    template<typename T, std::size_t N>
    class Segment {
    public:
        Segment(Point<T, N> start, Point<T, N> end)
            : start_(std::move(start)), end_(std::move(end)) {}

        [[nodiscard]] const Point<T, N>& start() const noexcept { return start_; }
        [[nodiscard]] const Point<T, N>& end()   const noexcept { return end_; }

        /// 线段长度 / Segment length
        [[nodiscard]] T length() const { return start_.distance_to(end_); }

        /// 线段中点 / Segment midpoint
        [[nodiscard]] Point<T, N> midpoint() const { return start_.midpoint(end_); }

    private:
        Point<T, N> start_;
        Point<T, N> end_;
    };

    template<typename T>
    using Segment2 = Segment<T, 2>;

    template<typename T>
    using Segment3 = Segment<T, 3>;

    using Segment2d = Segment2<double>;
    using Segment3d = Segment3<double>;

}  // namespace ospf::math::geometry
