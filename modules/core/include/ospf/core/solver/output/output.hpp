#pragma once
/// Solver output /// 1:1 ∂‘”¶ Rust/core/solver/output.rs
#include <string>
#include <unordered_map>
namespace ospf::core {
    struct SolverOutput {
        int status_code = -1;
        std::string status_message;
        double objective_value = 0.0;
        std::unordered_map<std::string, double> variable_values;
        std::unordered_map<std::string, double> dual_values;
    };
}