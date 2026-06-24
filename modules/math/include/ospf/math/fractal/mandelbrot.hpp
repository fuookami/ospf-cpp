#pragma once
/// Mandelbrot 集合 / Mandelbrot set /// 1:1 对应 Rust fractal/mandelbrot.rs
#include <complex>
#include <cstddef>
namespace ospf::math::fractal {
    template<typename S = double>
    [[nodiscard]] std::size_t mandelbrot_iterations(
        std::complex<S> c, std::size_t max_iter, S escape_radius = S{2})
    {
        std::complex<S> z{0, 0};
        for (std::size_t i = 0; i < max_iter; ++i) {
            z = z * z + c;
            if (std::abs(z) > escape_radius) return i;
        }
        return max_iter;
    }
}
