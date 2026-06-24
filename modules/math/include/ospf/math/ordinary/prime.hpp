#pragma once
/// 质数判断 / Prime check /// 1:1 对应 Rust ordinary/prime.rs
#include <cstdint>
namespace ospf::math::ordinary {
    [[nodiscard]] inline bool is_prime(std::int64_t n) {
        if (n < 2) return false;
        if (n < 4) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (std::int64_t i = 5; i * i <= n; i += 6)
            if (n % i == 0 || n % (i + 2) == 0) return false;
        return true;
    }
}
