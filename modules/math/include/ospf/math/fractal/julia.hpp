#pragma once
/// Julia 集合 / Julia set /// 1:1 对应 Rust fractal/julia.rs
#include <complex>
#include <cstddef>
namespace ospf::math::fractal {
    template<typename S = double>
    [[nodiscard]] std::size_t julia_iterations(
        std::complex<S> z, std::complex<S> c, std::size_t max_iter, S escape_radius = S{2})
    {
        for (std::size_t i = 0; i < max_iter; ++i) {
            z = z * z + c;
            if (std::abs(z) > escape_radius) return i;
        }
        return max_iter;
    }
}
