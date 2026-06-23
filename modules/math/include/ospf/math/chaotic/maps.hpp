#pragma once

/// 其他混沌映射 / Other chaotic maps

#include <cmath>
#include <numbers>

namespace ospf::math::chaotic {

    /// Tent 映射 / Tent map
    [[nodiscard]] inline double tent_map(double x, double mu = 2.0) {
        return x < 0.5 ? mu * x : mu * (1.0 - x);
    }

    /// Sine 映射 / Sine map
    [[nodiscard]] inline double sine_map(double x, double r = 1.0) {
        return r * std::sin(std::numbers::pi * x);
    }

    /// Circle 映射 / Circle map
    [[nodiscard]] inline double circle_map(double x, double omega = 0.5, double k = 1.0) {
        return std::fmod(x + omega - k / (2.0 * std::numbers::pi) * std::sin(2.0 * std::numbers::pi * x), 1.0);
    }

    /// Gauss 映射 / Gauss map
    [[nodiscard]] inline double gauss_map(double x, double beta = 0.5) {
        if (x == 0.0) return 0.0;
        return std::fmod(1.0 / x, 1.0) + beta;
    }

    /// Dyadic 映射 / Dyadic map (Bernoulli shift)
    [[nodiscard]] inline double dyadic_map(double x) {
        return std::fmod(2.0 * x, 1.0);
    }

}  // namespace ospf::math::chaotic
