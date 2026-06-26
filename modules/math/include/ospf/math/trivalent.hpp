#pragma once
/// 三值逻辑 / Trivalent logic
/// 1:1 对应 Rust math/trivalent.rs
/// 支持 True/False/Unknown 三值逻辑运算

#include <cstdint>
#include <ostream>

namespace ospf::math {

    /// 三值逻辑枚举 / Trivalent logic enum
    enum class Trivalent : std::uint8_t {
        False = 0,
        True = 1,
        Unknown = 2,
    };

    /// 逻辑与 / Logical AND
    [[nodiscard]] constexpr Trivalent operator&&(Trivalent a, Trivalent b) noexcept {
        if (a == Trivalent::False || b == Trivalent::False) return Trivalent::False;
        if (a == Trivalent::Unknown || b == Trivalent::Unknown) return Trivalent::Unknown;
        return Trivalent::True;
    }

    /// 逻辑或 / Logical OR
    [[nodiscard]] constexpr Trivalent operator||(Trivalent a, Trivalent b) noexcept {
        if (a == Trivalent::True || b == Trivalent::True) return Trivalent::True;
        if (a == Trivalent::Unknown || b == Trivalent::Unknown) return Trivalent::Unknown;
        return Trivalent::False;
    }

    /// 逻辑非 / Logical NOT
    [[nodiscard]] constexpr Trivalent operator!(Trivalent a) noexcept {
        if (a == Trivalent::True) return Trivalent::False;
        if (a == Trivalent::False) return Trivalent::True;
        return Trivalent::Unknown;
    }

    /// 是否为真 / Is true
    [[nodiscard]] constexpr bool is_true(Trivalent v) noexcept {
        return v == Trivalent::True;
    }

    /// 是否为假 / Is false
    [[nodiscard]] constexpr bool is_false(Trivalent v) noexcept {
        return v == Trivalent::False;
    }

    /// 是否未知 / Is unknown
    [[nodiscard]] constexpr bool is_unknown(Trivalent v) noexcept {
        return v == Trivalent::Unknown;
    }

    /// 从 bool 转换 / Convert from bool
    [[nodiscard]] constexpr Trivalent from_bool(bool v) noexcept {
        return v ? Trivalent::True : Trivalent::False;
    }

    /// 输出流 / Output stream
    inline std::ostream& operator<<(std::ostream& os, Trivalent v) {
        switch (v) {
            case Trivalent::True: return os << "True";
            case Trivalent::False: return os << "False";
            case Trivalent::Unknown: return os << "Unknown";
        }
        return os << "Unknown";
    }

}  // namespace ospf::math
