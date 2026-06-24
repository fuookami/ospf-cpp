#pragma once
/// 大数幂运算 / Big decimal power /// 1:1 对应 Rust ordinary/big_decimal_pow.rs
#include <cmath>
namespace ospf::math::ordinary {
    template<typename T>
    [[nodiscard]] T pow_int(T base, int exp) {
        T result = T{1};
        for (int i = 0; i < exp; ++i) result *= base;
        return result;
    }
}
