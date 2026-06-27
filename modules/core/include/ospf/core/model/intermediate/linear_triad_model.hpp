#pragma once
/// Linear triad model /// 1:1 ∂‘”¶ Rust core/model/intermediate/linear_triad_model.rs
#include <vector>
#include <unordered_map>
namespace ospf::core {
    struct LinearTriadModel {
        std::vector<std::pair<std::size_t, double>> objective_coefficients;
        std::vector<std::vector<std::pair<std::size_t, double>>> constraint_coefficients;
        std::vector<double> constraint_rhs;
        std::vector<int> constraint_sense;  // -1: <=, 0: =, 1: >=
    };
}