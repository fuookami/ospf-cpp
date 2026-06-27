#pragma once
/// Basic linear triad model /// 1:1 ∂‘”¶ Rust core/model/intermediate/basic_linear_triad_model.rs
#include <vector>
#include <unordered_map>
namespace ospf::core {
    struct BasicLinearTriadModel {
        std::vector<std::pair<std::size_t, double>> objective;
        std::vector<std::vector<std::pair<std::size_t, double>>> constraints;
        std::vector<double> rhs;
        std::vector<int> sense;
    };
}