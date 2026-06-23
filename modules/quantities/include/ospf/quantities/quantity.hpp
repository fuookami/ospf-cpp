#pragma once

/// 物理量 / Physical quantity
/// Quantity<V, Dim> 将数值 V 与维度 Dim 绑定，编译期防止不同维度混算。

#include <ospf/quantities/dimension.hpp>
#include <ospf/quantities/unit.hpp>
#include <array>
#include <compare>
#include <ostream>

namespace ospf::quantities {

    /// 物理量 / Physical quantity: value with dimension
    template<typename V, Dimension Dim>
    class Quantity {
    public:
        using value_type = V;

        /// 默认构造（零值） / Default construct (zero value)
        constexpr Quantity() : value_(V{}) {}

        /// 从数值构造 / Construct from value
        explicit constexpr Quantity(V value) : value_(std::move(value)) {}

        // -- 访问 / Access ------------------------------------------------------

        [[nodiscard]] constexpr const V& value() const noexcept { return value_; }
        [[nodiscard]] static constexpr Dimension dimension() noexcept { return Dim; }

        // -- 算术运算 / Arithmetic operations ----------------------------------

        [[nodiscard]] constexpr Quantity operator+(const Quantity& other) const {
            return Quantity(value_ + other.value_);
        }

        [[nodiscard]] constexpr Quantity operator-(const Quantity& other) const {
            return Quantity(value_ - other.value_);
        }

        [[nodiscard]] constexpr Quantity operator-() const {
            return Quantity(-value_);
        }

        [[nodiscard]] constexpr Quantity operator*(const V& scalar) const {
            return Quantity(value_ * scalar);
        }

        [[nodiscard]] friend constexpr Quantity operator*(const V& scalar, const Quantity& q) {
            return Quantity(scalar * q.value_);
        }

        [[nodiscard]] constexpr Quantity operator/(const V& scalar) const {
            return Quantity(value_ / scalar);
        }

        /// 两个同维度量相除得到无量纲数 / Division of same-dimension quantities gives dimensionless
        [[nodiscard]] constexpr V operator/(const Quantity& other) const {
            return value_ / other.value_;
        }

        // -- 比较 / Comparison -------------------------------------------------

        [[nodiscard]] constexpr bool operator==(const Quantity& other) const noexcept = default;
        [[nodiscard]] constexpr auto operator<=>(const Quantity& other) const noexcept = default;

        // -- 输出 / Output -----------------------------------------------------

        friend std::ostream& operator<<(std::ostream& os, const Quantity& q) {
            return os << q.value_;
        }

    private:
        V value_;
    };

    // -- 标量物理量别名 / Scalar quantity aliases ------------------------------

    template<Dimension Dim>
    using ScalarQuantity = Quantity<double, Dim>;

    // -- 常用物理量类型 / Common quantity types --------------------------------

    using Length       = ScalarQuantity<DIM_LENGTH>;
    using Mass         = ScalarQuantity<DIM_MASS>;
    using Time         = ScalarQuantity<DIM_TIME>;
    using Velocity     = ScalarQuantity<DIM_VELOCITY>;
    using Acceleration = ScalarQuantity<DIM_ACCELERATION>;
    using Force        = ScalarQuantity<DIM_FORCE>;
    using Energy       = ScalarQuantity<DIM_ENERGY>;
    using Power        = ScalarQuantity<DIM_POWER>;
    using Area         = ScalarQuantity<DIM_AREA>;
    using Volume       = ScalarQuantity<DIM_VOLUME>;

    // -- 物理量向量（多维） / Quantity vector (multi-dimensional) ---------------

    template<typename V, Dimension Dim, std::size_t N>
    class QuantityVector {
    public:
        constexpr QuantityVector() : values_{} {}

        template<typename... Args>
        explicit constexpr QuantityVector(Args... args) : values_{static_cast<V>(args)...} {}

        [[nodiscard]] constexpr V& operator[](std::size_t i) { return values_[i]; }
        [[nodiscard]] constexpr const V& operator[](std::size_t i) const { return values_[i]; }
        [[nodiscard]] static constexpr std::size_t size() noexcept { return N; }

        [[nodiscard]] constexpr QuantityVector operator+(const QuantityVector& other) const {
            QuantityVector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.values_[i] = values_[i] + other.values_[i];
            }
            return result;
        }

        [[nodiscard]] constexpr QuantityVector operator-(const QuantityVector& other) const {
            QuantityVector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.values_[i] = values_[i] - other.values_[i];
            }
            return result;
        }

        [[nodiscard]] constexpr QuantityVector operator*(const V& scalar) const {
            QuantityVector result;
            for (std::size_t i = 0; i < N; ++i) {
                result.values_[i] = values_[i] * scalar;
            }
            return result;
        }

    private:
        std::array<V, N> values_;
    };

    // -- 单位换算辅助 / Unit conversion helpers --------------------------------

    /// 同维度不同单位的换算 / Convert between units of same dimension
    template<Dimension Dim>
    [[nodiscard]] constexpr ScalarQuantity<Dim> convert_unit(
        ScalarQuantity<Dim> value, double from_factor, double to_factor)
    {
        return ScalarQuantity<Dim>(value.value() * from_factor / to_factor);
    }

}  // namespace ospf::quantities
