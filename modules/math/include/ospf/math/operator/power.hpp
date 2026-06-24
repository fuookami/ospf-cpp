#pragma once
/// 幂函数 / Power function /// 1:1 对应 Rust operator/power.rs
#include <cmath>
namespace ospf::math::op {
    template<typename T> [[nodiscard]] T pow_int(T base, int exp) {
        T r = T{1}; for (int i = 0; i < exp; ++i) r *= base; return r;
    }
}
