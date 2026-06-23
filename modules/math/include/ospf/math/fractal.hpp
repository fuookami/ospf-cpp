#pragma once

/// 分形 / Fractals
/// Mandelbrot 集、Julia 集。

#include <cmath>
#include <complex>
#include <cstddef>
#include <vector>

namespace ospf::math {

    /// Mandelbrot 集迭代次数 / Mandelbrot set iteration count
    [[nodiscard]] inline std::size_t mandelbrot_iterations(
        std::complex<double> c, std::size_t max_iter = 100, double escape_radius = 2.0)
    {
        std::complex<double> z = 0.0;
        for (std::size_t i = 0; i < max_iter; ++i) {
            z = z * z + c;
            if (std::abs(z) > escape_radius) {
                return i + 1;
            }
        }
        return max_iter;  // 在集合内 / In the set
    }

    /// 检查点是否在 Mandelbrot 集内 / Check if point is in Mandelbrot set
    [[nodiscard]] inline bool is_in_mandelbrot(
        std::complex<double> c, std::size_t max_iter = 100, double escape_radius = 2.0)
    {
        return mandelbrot_iterations(c, max_iter, escape_radius) == max_iter;
    }

    /// Julia 集迭代次数 / Julia set iteration count
    [[nodiscard]] inline std::size_t julia_iterations(
        std::complex<double> z, std::complex<double> c,
        std::size_t max_iter = 100, double escape_radius = 2.0)
    {
        for (std::size_t i = 0; i < max_iter; ++i) {
            z = z * z + c;
            if (std::abs(z) > escape_radius) {
                return i + 1;
            }
        }
        return max_iter;
    }

    /// 检查点是否在 Julia 集内 / Check if point is in Julia set
    [[nodiscard]] inline bool is_in_julia(
        std::complex<double> z, std::complex<double> c,
        std::size_t max_iter = 100, double escape_radius = 2.0)
    {
        return julia_iterations(z, c, max_iter, escape_radius) == max_iter;
    }

    /// 生成 Mandelbrot 集网格 / Generate Mandelbrot set grid
    [[nodiscard]] inline std::vector<std::vector<std::size_t>> mandelbrot_grid(
        double x_min, double x_max, double y_min, double y_max,
        std::size_t width, std::size_t height,
        std::size_t max_iter = 100)
    {
        std::vector<std::vector<std::size_t>> grid(height, std::vector<std::size_t>(width));
        double dx = (x_max - x_min) / static_cast<double>(width);
        double dy = (y_max - y_min) / static_cast<double>(height);

        for (std::size_t row = 0; row < height; ++row) {
            for (std::size_t col = 0; col < width; ++col) {
                double x = x_min + static_cast<double>(col) * dx;
                double y = y_min + static_cast<double>(row) * dy;
                grid[row][col] = mandelbrot_iterations({x, y}, max_iter);
            }
        }
        return grid;
    }

}  // namespace ospf::math
