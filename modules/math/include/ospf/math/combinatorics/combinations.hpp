#pragma once
/// 组合数 / Combinations /// 1:1 对应 Rust combinatorics/combinations.rs
#include <cstdint>
namespace ospf::math::combinatorics {
    [[nodiscard]] constexpr std::uint64_t combination(std::uint64_t n, std::uint64_t k) {
        if (k > n) return 0;
        if (k == 0 || k == n) return 1;
        if (k > n - k) k = n - k;
        std::uint64_t result = 1;
        for (std::uint64_t i = 0; i < k; ++i) {
            result = result * (n - i) / (i + 1);
        }
        return result;
    }
}
