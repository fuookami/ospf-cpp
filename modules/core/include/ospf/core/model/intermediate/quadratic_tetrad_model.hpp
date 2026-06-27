#pragma once
/// Quadratic tetrad model /// 1:1 ∂‘”¶ Rust core/model/intermediate/quadratic_tetrad_model.rs
#include <vector>
#include <unordered_map>
namespace ospf::core {
    struct QuadraticTetradModel {
        std::vector<std::pair<std::size_t, double>> linear_objective;
        std::vector<std::tuple<std::size_t, std::size_t, double>> quadratic_objective;
        std::vector<std::vector<std::pair<std::size_t, double>>> constraint_coefficients;
        std::vector<double> constraint_rhs;
    };
}