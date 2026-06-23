#pragma once

/// 组合数学 / Combinatorics
/// 排列、组合、笛卡尔积、Newton 迭代。

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

namespace ospf::math {

    /// 阶乘 / Factorial
    [[nodiscard]] constexpr std::int64_t factorial(std::int64_t n) noexcept {
        std::int64_t result = 1;
        for (std::int64_t i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    /// 排列数 P(n, k) / Permutations P(n, k)
    [[nodiscard]] constexpr std::int64_t permutations(std::int64_t n, std::int64_t k) noexcept {
        std::int64_t result = 1;
        for (std::int64_t i = n - k + 1; i <= n; ++i) {
            result *= i;
        }
        return result;
    }

    /// 组合数 C(n, k) / Combinations C(n, k)
    [[nodiscard]] constexpr std::int64_t combinations(std::int64_t n, std::int64_t k) noexcept {
        if (k > n - k) k = n - k;
        std::int64_t result = 1;
        for (std::int64_t i = 0; i < k; ++i) {
            result = result * (n - i) / (i + 1);
        }
        return result;
    }

    /// 生成所有排列 / Generate all permutations
    template<typename T>
    [[nodiscard]] std::vector<std::vector<T>> all_permutations(std::vector<T> items) {
        std::vector<std::vector<T>> result;
        std::sort(items.begin(), items.end());
        do {
            result.push_back(items);
        } while (std::next_permutation(items.begin(), items.end()));
        return result;
    }

    /// 生成所有 k-组合 / Generate all k-combinations
    template<typename T>
    [[nodiscard]] std::vector<std::vector<T>> all_combinations(
        const std::vector<T>& items, std::size_t k)
    {
        std::vector<std::vector<T>> result;
        std::vector<bool> selector(items.size(), false);
        std::fill(selector.begin(), selector.begin() + k, true);

        do {
            std::vector<T> combo;
            for (std::size_t i = 0; i < items.size(); ++i) {
                if (selector[i]) combo.push_back(items[i]);
            }
            result.push_back(std::move(combo));
        } while (std::prev_permutation(selector.begin(), selector.end()));

        return result;
    }

    /// 笛卡尔积 / Cartesian product
    template<typename T>
    [[nodiscard]] std::vector<std::pair<T, T>> cartesian_product(
        const std::vector<T>& a, const std::vector<T>& b)
    {
        std::vector<std::pair<T, T>> result;
        for (const auto& x : a) {
            for (const auto& y : b) {
                result.emplace_back(x, y);
            }
        }
        return result;
    }

    /// Newton 迭代法 / Newton's method
    template<typename F, typename DF>
    [[nodiscard]] double newton_iterate(
        F f, DF df, double x0, std::size_t max_iter = 100, double tol = 1e-10)
    {
        double x = x0;
        for (std::size_t i = 0; i < max_iter; ++i) {
            double fx = f(x);
            if (std::abs(fx) < tol) break;
            double dfx = df(x);
            if (dfx == 0.0) break;
            x = x - fx / dfx;
        }
        return x;
    }

}  // namespace ospf::math
