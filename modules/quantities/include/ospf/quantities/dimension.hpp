#pragma once

/// 物理维度 / Physical dimension
/// 表示物理量的维度组合（质量、长度、时间等）。

#include <cstdint>
#include <ostream>

namespace ospf::quantities {

    /// 物理维度：7 个基本量的指数 / Physical dimension: exponents of 7 base quantities
    struct Dimension {
        std::int8_t mass        = 0;  ///< 质量 M / Mass
        std::int8_t length      = 0;  ///< 长度 L / Length
        std::int8_t time        = 0;  ///< 时间 T / Time
        std::int8_t current     = 0;  ///< 电流 I / Current
        std::int8_t temperature = 0;  ///< 温度 Θ / Temperature
        std::int8_t amount      = 0;  ///< 物质的量 N / Amount of substance
        std::int8_t luminosity  = 0;  ///< 发光强度 J / Luminous intensity

        [[nodiscard]] constexpr bool operator==(const Dimension&) const = default;

        /// 是否为无量纲 / Is dimensionless
        [[nodiscard]] constexpr bool is_dimensionless() const noexcept {
            return mass == 0 && length == 0 && time == 0 && current == 0
                && temperature == 0 && amount == 0 && luminosity == 0;
        }

        /// 乘法：维度相加 / Multiplication: add dimensions
        [[nodiscard]] constexpr Dimension operator*(const Dimension& other) const noexcept {
            return {
                static_cast<std::int8_t>(mass + other.mass),
                static_cast<std::int8_t>(length + other.length),
                static_cast<std::int8_t>(time + other.time),
                static_cast<std::int8_t>(current + other.current),
                static_cast<std::int8_t>(temperature + other.temperature),
                static_cast<std::int8_t>(amount + other.amount),
                static_cast<std::int8_t>(luminosity + other.luminosity)
            };
        }

        /// 除法：维度相减 / Division: subtract dimensions
        [[nodiscard]] constexpr Dimension operator/(const Dimension& other) const noexcept {
            return {
                static_cast<std::int8_t>(mass - other.mass),
                static_cast<std::int8_t>(length - other.length),
                static_cast<std::int8_t>(time - other.time),
                static_cast<std::int8_t>(current - other.current),
                static_cast<std::int8_t>(temperature - other.temperature),
                static_cast<std::int8_t>(amount - other.amount),
                static_cast<std::int8_t>(luminosity - other.luminosity)
            };
        }

        /// 幂：维度乘以标量 / Power: multiply dimensions by scalar
        [[nodiscard]] constexpr Dimension pow(int n) const noexcept {
            return {
                static_cast<std::int8_t>(mass * n),
                static_cast<std::int8_t>(length * n),
                static_cast<std::int8_t>(time * n),
                static_cast<std::int8_t>(current * n),
                static_cast<std::int8_t>(temperature * n),
                static_cast<std::int8_t>(amount * n),
                static_cast<std::int8_t>(luminosity * n)
            };
        }

        friend std::ostream& operator<<(std::ostream& os, const Dimension& d) {
            if (d.is_dimensionless()) return os << "1";
            auto print_term = [&](const char* sym, std::int8_t exp) {
                if (exp != 0) {
                    os << sym;
                    if (exp != 1) os << "^" << static_cast<int>(exp);
                }
            };
            print_term("M", d.mass);
            print_term("L", d.length);
            print_term("T", d.time);
            print_term("I", d.current);
            print_term("Θ", d.temperature);
            print_term("N", d.amount);
            print_term("J", d.luminosity);
            return os;
        }
    };

    // -- 预定义维度 / Predefined dimensions -----------------------------------

    inline constexpr Dimension DIMENSIONLESS = {};
    inline constexpr Dimension DIM_MASS       = {1, 0, 0, 0, 0, 0, 0};
    inline constexpr Dimension DIM_LENGTH     = {0, 1, 0, 0, 0, 0, 0};
    inline constexpr Dimension DIM_TIME       = {0, 0, 1, 0, 0, 0, 0};
    inline constexpr Dimension DIM_CURRENT    = {0, 0, 0, 1, 0, 0, 0};
    inline constexpr Dimension DIM_TEMPERATURE = {0, 0, 0, 0, 1, 0, 0};
    inline constexpr Dimension DIM_AMOUNT     = {0, 0, 0, 0, 0, 1, 0};
    inline constexpr Dimension DIM_LUMINOSITY = {0, 0, 0, 0, 0, 0, 1};

    // -- 派生维度 / Derived dimensions ----------------------------------------

    inline constexpr Dimension DIM_VELOCITY   = {0, 1, -1, 0, 0, 0, 0};      ///< m/s
    inline constexpr Dimension DIM_ACCELERATION = {0, 1, -2, 0, 0, 0, 0};    ///< m/s^2
    inline constexpr Dimension DIM_FORCE      = {1, 1, -2, 0, 0, 0, 0};      ///< N = kg*m/s^2
    inline constexpr Dimension DIM_ENERGY     = {1, 2, -2, 0, 0, 0, 0};      ///< J = kg*m^2/s^2
    inline constexpr Dimension DIM_POWER      = {1, 2, -3, 0, 0, 0, 0};      ///< W = kg*m^2/s^3
    inline constexpr Dimension DIM_AREA       = {0, 2, 0, 0, 0, 0, 0};       ///< m^2
    inline constexpr Dimension DIM_VOLUME     = {0, 3, 0, 0, 0, 0, 0};       ///< m^3

}  // namespace ospf::quantities
