#pragma once
/// 最大公约数 / GCD /// 1:1 对应 Rust ordinary/gcd.rs
#include <type_traits>
namespace ospf::math::ordinary {
    template<typename T>
    [[nodiscard]] constexpr T gcd(T a, T b) noexcept {
        while (b != T{0}) { T t = b; b = a % b; a = t; }
        return a;
    }
}
