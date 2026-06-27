#pragma once
/// Solver output /// 1:1 ∂‘”¶ Rust/core/solver/output/solver_output.rs
#include <string>
#include <unordered_map>
namespace ospf::core {
    struct SolverOutputDetail {
        int status_code = -1;
        std::string status_message;
        double objective_value = 0.0;
        std::unordered_map<std::string, double> variable_values;
    };
}