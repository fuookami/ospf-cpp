#pragma once

/// Logistic 映射 / Logistic map

namespace ospf::math::chaotic {

    /// Logistic 映射: x_{n+1} = r * x_n * (1 - x_n)
    [[nodiscard]] inline double logistic_map(double x, double r) {
        return r * x * (1.0 - x);
    }

    /// 迭代 N 步 / Iterate N steps
    [[nodiscard]] inline double logistic_iterate(double x, double r, std::size_t steps) {
        for (std::size_t i = 0; i < steps; ++i) {
            x = logistic_map(x, r);
        }
        return x;
    }

}  // namespace ospf::math::chaotic
