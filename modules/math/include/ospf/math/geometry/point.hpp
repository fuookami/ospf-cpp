#pragma once

/// 点类型 / Point type
/// N 维空间中的点。

#include <array>
#include <cmath>
#include <cstddef>
#include <ostream>

namespace ospf::math::geometry {

    /// N 维点 / N-dimensional point
    template<typename T, std::size_t N>
    class Point {
    public:
        constexpr Point() : data_{} {}

        explicit constexpr Point(const std::array<T, N>& data) : data_(data) {}

        constexpr Point(std::initializer_list<T> init) {
            std::size_t i = 0;
            for (auto it = init.begin(); it != init.end() && i < N; ++it, ++i) {
                data_[i] = *it;
            }
        }

        [[nodiscard]] constexpr T& operator[](std::size_t i) { return data_[i]; }
        [[nodiscard]] constexpr const T& operator[](std::size_t i) const { return data_[i]; }
        [[nodiscard]] constexpr std::size_t dimension() const noexcept { return N; }

        /// 便捷访问器（2D/3D）/ Convenience accessors (2D/3D)
        [[nodiscard]] constexpr T& x() requires(N >= 1) { return data_[0]; }
        [[nodiscard]] constexpr const T& x() const requires(N >= 1) { return data_[0]; }
        [[nodiscard]] constexpr T& y() requires(N >= 2) { return data_[1]; }
        [[nodiscard]] constexpr const T& y() const requires(N >= 2) { return data_[1]; }
        [[nodiscard]] constexpr T& z() requires(N >= 3) { return data_[2]; }
        [[nodiscard]] constexpr const T& z() const requires(N >= 3) { return data_[2]; }

        /// 计算到另一点的距离 / Calculate distance to another point
        [[nodiscard]] T distance_to(const Point& other) const {
            T sum = T{};
            for (std::size_t i = 0; i < N; ++i) {
                T diff = data_[i] - other.data_[i];
                sum += diff * diff;
            }
            return std::sqrt(sum);
        }

        /// 计算到另一点的距离平方 / Calculate squared distance to another point
        [[nodiscard]] constexpr T distance_squared_to(const Point& other) const {
            T sum = T{};
            for (std::size_t i = 0; i < N; ++i) {
                T diff = data_[i] - other.data_[i];
                sum += diff * diff;
            }
            return sum;
        }

        /// 中点 / Midpoint
        [[nodiscard]] constexpr Point midpoint(const Point& other) const {
            Point result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = (data_[i] + other.data_[i]) / T{2};
            }
            return result;
        }

        [[nodiscard]] constexpr bool operator==(const Point& other) const = default;
        [[nodiscard]] constexpr auto operator<=>(const Point& other) const = default;

        friend std::ostream& operator<<(std::ostream& os, const Point& p) {
            os << "Point(";
            for (std::size_t i = 0; i < N; ++i) {
                if (i > 0) os << ", ";
                os << p.data_[i];
            }
            os << ")";
            return os;
        }

    private:
        std::array<T, N> data_;
    };

    template<typename T>
    using Point2 = Point<T, 2>;

    template<typename T>
    using Point3 = Point<T, 3>;

    using Point2d = Point2<double>;
    using Point3d = Point3<double>;
    using Point2f = Point2<float>;
    using Point3f = Point3<float>;

}  // namespace ospf::math::geometry
