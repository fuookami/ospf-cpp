#pragma once

/// 三角形 / Triangle

#include <ospf/math/geometry/point.hpp>
#include <cmath>

namespace ospf::math::geometry {

    /// 三角形 / Triangle
    template<typename T>
    class Triangle {
    public:
        Triangle(Point2<T> a, Point2<T> b, Point2<T> c)
            : a_(std::move(a)), b_(std::move(b)), c_(std::move(c)) {}

        [[nodiscard]] const Point2<T>& a() const noexcept { return a_; }
        [[nodiscard]] const Point2<T>& b() const noexcept { return b_; }
        [[nodiscard]] const Point2<T>& c() const noexcept { return c_; }

        /// 边长 / Edge lengths
        [[nodiscard]] T ab() const { return a_.distance_to(b_); }
        [[nodiscard]] T bc() const { return b_.distance_to(c_); }
        [[nodiscard]] T ca() const { return c_.distance_to(a_); }

        /// 周长 / Perimeter
        [[nodiscard]] T perimeter() const { return ab() + bc() + ca(); }

        /// 面积（海伦公式） / Area (Heron's formula)
        [[nodiscard]] T area() const {
            T s = perimeter() / T{2};
            T a = ab(), b = bc(), c = ca();
            return std::sqrt(s * (s - a) * (s - b) * (s - c));
        }

        /// 重心 / Centroid
        [[nodiscard]] Point2<T> centroid() const {
            return Point2<T>({
                (a_[0] + b_[0] + c_[0]) / T{3},
                (a_[1] + b_[1] + c_[1]) / T{3}
            });
        }

    private:
        Point2<T> a_, b_, c_;
    };

    using Triangle2d = Triangle<double>;
    using Triangle2f = Triangle<float>;

}  // namespace ospf::math::geometry
