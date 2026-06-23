#pragma once

/// 圆 / Circle

#include <ospf/math/geometry/point.hpp>
#include <numbers>

namespace ospf::math::geometry {

    /// 圆 / Circle
    template<typename T>
    class Circle {
    public:
        Circle(Point2<T> center, T radius)
            : center_(std::move(center)), radius_(radius) {}

        [[nodiscard]] const Point2<T>& center() const noexcept { return center_; }
        [[nodiscard]] T radius() const noexcept { return radius_; }

        /// 周长 / Circumference
        [[nodiscard]] T circumference() const {
            return T{2} * std::numbers::pi_v<T> * radius_;
        }

        /// 面积 / Area
        [[nodiscard]] T area() const {
            return std::numbers::pi_v<T> * radius_ * radius_;
        }

        /// 点是否在圆内 / Check if point is inside circle
        [[nodiscard]] bool contains(const Point2<T>& point) const {
            return center_.distance_squared_to(point) <= radius_ * radius_;
        }

    private:
        Point2<T> center_;
        T radius_;
    };

    using Circle2d = Circle<double>;
    using Circle2f = Circle<float>;

}  // namespace ospf::math::geometry
