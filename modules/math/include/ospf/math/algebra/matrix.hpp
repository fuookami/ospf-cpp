#pragma once

/// 矩阵类型 / Matrix type
/// 固定大小的数学矩阵，支持加减、标量乘、矩阵乘、转置。

#include <ospf/math/algebra/vector.hpp>
#include <array>
#include <cstddef>
#include <ostream>

namespace ospf::math::algebra {

    /// 固定大小数学矩阵 / Fixed-size mathematical matrix
    template<typename T, std::size_t Rows, std::size_t Cols>
    class Matrix {
    public:
        /// 默认构造（零矩阵） / Default construct (zero matrix)
        constexpr Matrix() : data_{} {}

        /// 从数组构造 / Construct from array of rows
        explicit constexpr Matrix(const std::array<std::array<T, Cols>, Rows>& data) : data_(data) {}

        /// 零矩阵 / Zero matrix
        [[nodiscard]] static constexpr Matrix zero() noexcept {
            return Matrix{};
        }

        /// 单位矩阵 / Identity matrix (only for square matrices)
        [[nodiscard]] static constexpr Matrix identity() requires(Rows == Cols) {
            Matrix m;
            for (std::size_t i = 0; i < Rows; ++i) {
                m.data_[i][i] = T{1};
            }
            return m;
        }

        // -- 元素访问 / Element access ----------------------------------------

        [[nodiscard]] constexpr T& at(std::size_t r, std::size_t c) { return data_[r][c]; }
        [[nodiscard]] constexpr const T& at(std::size_t r, std::size_t c) const { return data_[r][c]; }

        [[nodiscard]] constexpr std::size_t rows() const noexcept { return Rows; }
        [[nodiscard]] constexpr std::size_t cols() const noexcept { return Cols; }

        // -- 算术运算 / Arithmetic operations ----------------------------------

        [[nodiscard]] constexpr Matrix operator+(const Matrix& other) const {
            Matrix result;
            for (std::size_t r = 0; r < Rows; ++r) {
                for (std::size_t c = 0; c < Cols; ++c) {
                    result.data_[r][c] = data_[r][c] + other.data_[r][c];
                }
            }
            return result;
        }

        [[nodiscard]] constexpr Matrix operator-(const Matrix& other) const {
            Matrix result;
            for (std::size_t r = 0; r < Rows; ++r) {
                for (std::size_t c = 0; c < Cols; ++c) {
                    result.data_[r][c] = data_[r][c] - other.data_[r][c];
                }
            }
            return result;
        }

        [[nodiscard]] constexpr Matrix operator*(T scalar) const {
            Matrix result;
            for (std::size_t r = 0; r < Rows; ++r) {
                for (std::size_t c = 0; c < Cols; ++c) {
                    result.data_[r][c] = data_[r][c] * scalar;
                }
            }
            return result;
        }

        [[nodiscard]] friend constexpr Matrix operator*(T scalar, const Matrix& m) {
            return m * scalar;
        }

        /// 矩阵乘法 / Matrix multiplication
        template<std::size_t OtherCols>
        [[nodiscard]] constexpr Matrix<T, Rows, OtherCols> operator*(
            const Matrix<T, Cols, OtherCols>& other) const
        {
            Matrix<T, Rows, OtherCols> result;
            for (std::size_t r = 0; r < Rows; ++r) {
                for (std::size_t c = 0; c < OtherCols; ++c) {
                    T sum = T{};
                    for (std::size_t k = 0; k < Cols; ++k) {
                        sum += data_[r][k] * other.at(k, c);
                    }
                    result.at(r, c) = sum;
                }
            }
            return result;
        }

        /// 矩阵-向量乘法 / Matrix-vector multiplication
        [[nodiscard]] constexpr Vector<T, Rows> operator*(const Vector<T, Cols>& v) const {
            Vector<T, Rows> result;
            for (std::size_t r = 0; r < Rows; ++r) {
                T sum = T{};
                for (std::size_t c = 0; c < Cols; ++c) {
                    sum += data_[r][c] * v[c];
                }
                result[r] = sum;
            }
            return result;
        }

        // -- 转置 / Transpose --------------------------------------------------

        [[nodiscard]] constexpr Matrix<T, Cols, Rows> transposed() const {
            Matrix<T, Cols, Rows> result;
            for (std::size_t r = 0; r < Rows; ++r) {
                for (std::size_t c = 0; c < Cols; ++c) {
                    result.at(c, r) = data_[r][c];
                }
            }
            return result;
        }

        // -- 行列式 / Determinant (square matrices only) -----------------------

        [[nodiscard]] constexpr T determinant() const requires(Rows == Cols && Rows == 1) {
            return data_[0][0];
        }

        [[nodiscard]] constexpr T determinant() const requires(Rows == Cols && Rows == 2) {
            return data_[0][0] * data_[1][1] - data_[0][1] * data_[1][0];
        }

        [[nodiscard]] constexpr T determinant() const requires(Rows == Cols && Rows == 3) {
            return data_[0][0] * (data_[1][1] * data_[2][2] - data_[1][2] * data_[2][1])
                 - data_[0][1] * (data_[1][0] * data_[2][2] - data_[1][2] * data_[2][0])
                 + data_[0][2] * (data_[1][0] * data_[2][1] - data_[1][1] * data_[2][0]);
        }

        // -- 比较 / Comparison -------------------------------------------------

        [[nodiscard]] constexpr bool operator==(const Matrix& other) const = default;

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
            for (std::size_t r = 0; r < Rows; ++r) {
                os << "[";
                for (std::size_t c = 0; c < Cols; ++c) {
                    if (c > 0) os << ", ";
                    os << m.data_[r][c];
                }
                os << "]\n";
            }
            return os;
        }

    private:
        std::array<std::array<T, Cols>, Rows> data_;
    };

    // -- 便捷别名 / Convenience aliases ---------------------------------------

    template<typename T>
    using Mat2 = Matrix<T, 2, 2>;

    template<typename T>
    using Mat3 = Matrix<T, 3, 3>;

    template<typename T>
    using Mat4 = Matrix<T, 4, 4>;

    using Mat2d = Mat2<double>;
    using Mat3d = Mat3<double>;
    using Mat4d = Mat4<double>;

    using Mat2f = Mat2<float>;
    using Mat3f = Mat3<float>;
    using Mat4f = Mat4<float>;

}  // namespace ospf::math::algebra
