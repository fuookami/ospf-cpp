#pragma once

/// 向量类型 / Vector type
/// 固定大小的数学向量，支持加减、标量乘、内积、范数。

#include <array>
#include <cmath>
#include <cstddef>
#include <ostream>

namespace ospf::math::algebra {

    /// 固定大小数学向量 / Fixed-size mathematical vector
    template<typename T, std::size_t N>
    class Vector {
    public:
        /// 默认构造（零向量） / Default construct (zero vector)
        constexpr Vector() : data_{} {}

        /// 从数组构造 / Construct from array
        explicit constexpr Vector(const std::array<T, N>& data) : data_(data) {}

        /// 从初始化列表构造 / Construct from initializer list
        constexpr Vector(std::initializer_list<T> init) {
            std::size_t i = 0;
            for (auto it = init.begin(); it != init.end() && i < N; ++it, ++i) {
                data_[i] = *it;
            }
        }

        /// 零向量 / Zero vector
        [[nodiscard]] static constexpr Vector zero() noexcept {
            return Vector{};
        }

        // -- 元素访问 / Element access ----------------------------------------

        [[nodiscard]] constexpr T& operator[](std::size_t i) { return data_[i]; }
        [[nodiscard]] constexpr const T& operator[](std::size_t i) const { return data_[i]; }

        [[nodiscard]] constexpr std::size_t size() const noexcept { return N; }

        [[nodiscard]] constexpr T* data() noexcept { return data_.data(); }
        [[nodiscard]] constexpr const T* data() const noexcept { return data_.data(); }

        // -- 算术运算 / Arithmetic operations ----------------------------------

        [[nodiscard]] constexpr Vector operator+(const Vector& other) const {
            Vector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = data_[i] + other.data_[i];
            }
            return result;
        }

        [[nodiscard]] constexpr Vector operator-(const Vector& other) const {
            Vector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = data_[i] - other.data_[i];
            }
            return result;
        }

        [[nodiscard]] constexpr Vector operator-() const {
            Vector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = -data_[i];
            }
            return result;
        }

        [[nodiscard]] constexpr Vector operator*(T scalar) const {
            Vector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = data_[i] * scalar;
            }
            return result;
        }

        [[nodiscard]] friend constexpr Vector operator*(T scalar, const Vector& v) {
            return v * scalar;
        }

        [[nodiscard]] constexpr Vector operator/(T scalar) const {
            Vector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.data_[i] = data_[i] / scalar;
            }
            return result;
        }

        // -- 内积 / Dot product ------------------------------------------------

        [[nodiscard]] constexpr T dot(const Vector& other) const {
            T sum = T{};
            for (std::size_t i = 0; i < N; ++i) {
                sum += data_[i] * other.data_[i];
            }
            return sum;
        }

        // -- 范数 / Norm -------------------------------------------------------

        [[nodiscard]] constexpr T norm_squared() const {
            return dot(*this);
        }

        [[nodiscard]] T norm() const {
            return std::sqrt(norm_squared());
        }

        /// 归一化 / Normalize
        [[nodiscard]] Vector normalized() const {
            T n = norm();
            return *this / n;
        }

        // -- 比较 / Comparison -------------------------------------------------

        [[nodiscard]] constexpr bool operator==(const Vector& other) const = default;
        [[nodiscard]] constexpr auto operator<=>(const Vector& other) const = default;

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
            os << "(";
            for (std::size_t i = 0; i < N; ++i) {
                if (i > 0) os << ", ";
                os << v.data_[i];
            }
            os << ")";
            return os;
        }

    private:
        std::array<T, N> data_;
    };

    // -- 便捷别名 / Convenience aliases ---------------------------------------

    template<typename T>
    using Vec2 = Vector<T, 2>;

    template<typename T>
    using Vec3 = Vector<T, 3>;

    template<typename T>
    using Vec4 = Vector<T, 4>;

    using Vec2d = Vec2<double>;
    using Vec3d = Vec3<double>;
    using Vec4d = Vec4<double>;

    using Vec2f = Vec2<float>;
    using Vec3f = Vec3<float>;
    using Vec4f = Vec4<float>;

}  // namespace ospf::math::algebra
