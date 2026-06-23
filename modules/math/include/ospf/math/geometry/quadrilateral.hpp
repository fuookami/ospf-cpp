#pragma once

/// 四边形 / Quadrilateral

#include <ospf/math/geometry/point.hpp>
#include <cmath>

namespace ospf::math::geometry {

    /// 四边形 / Quadrilateral
    template<typename T>
    class Quadrilateral {
    public:
        Quadrilateral(Point2<T> a, Point2<T> b, Point2<T> c, Point2<T> d)
            : a_(std::move(a)), b_(std::move(b)), c_(std::move(c)), d_(std::move(d)) {}

        [[nodiscard]] const Point2<T>& a() const noexcept { return a_; }
        [[nodiscard]] const Point2<T>& b() const noexcept { return b_; }
        [[nodiscard]] const Point2<T>& c() const noexcept { return c_; }
        [[nodiscard]] const Point2<T>& d() const noexcept { return d_; }

        /// 周长 / Perimeter
        [[nodiscard]] T perimeter() const {
            return a_.distance_to(b_) + b_.distance_to(c_) +
                   c_.distance_to(d_) + d_.distance_to(a_);
        }

        /// 面积（鞋带公式） / Area (Shoelace formula)
        [[nodiscard]] T area() const {
            T sum = a_[0] * b_[1] - b_[0] * a_[1]
                  + b_[0] * c_[1] - c_[0] * b_[1]
                  + c_[0] * d_[1] - d_[0] * c_[1]
                  + d_[0] * a_[1] - a_[0] * d_[1];
            return std::abs(sum) / T{2};
        }

    private:
        Point2<T> a_, b_, c_, d_;
    };

    using Quadrilateral2d = Quadrilateral<double>;
    using Quadrilateral2f = Quadrilateral<float>;

}  // namespace ospf::math::geometry
