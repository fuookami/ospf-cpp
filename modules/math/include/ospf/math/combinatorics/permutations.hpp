#pragma once
/// 排列数 / Permutations /// 1:1 对应 Rust combinatorics/permutations.rs
#include <cstdint>
namespace ospf::math::combinatorics {
    [[nodiscard]] constexpr std::uint64_t permutation(std::uint64_t n, std::uint64_t k) {
        if (k > n) return 0;
        std::uint64_t result = 1;
        for (std::uint64_t i = 0; i < k; ++i) { result *= (n - i); }
        return result;
    }
    [[nodiscard]] constexpr std::uint64_t factorial(std::uint64_t n) {
        return permutation(n, n);
    }
}
