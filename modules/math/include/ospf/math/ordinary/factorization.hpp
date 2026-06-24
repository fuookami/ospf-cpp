#pragma once
/// 因式分解 / Factorization /// 1:1 对应 Rust ordinary/factorization.rs
#include <vector>
#include <cstdint>
namespace ospf::math::ordinary {
    [[nodiscard]] inline std::vector<std::int64_t> prime_factors(std::int64_t n) {
        std::vector<std::int64_t> factors;
        for (std::int64_t d = 2; d * d <= n; ++d)
            while (n % d == 0) { factors.push_back(d); n /= d; }
        if (n > 1) factors.push_back(n);
        return factors;
    }
}
