#pragma once
/// Solution /// 1:1 ∂‘”¶ Rust/core/model/callback/solution.rs
#include <unordered_map>
#include <string>
namespace ospf::core {
    struct Solution {
        std::unordered_map<std::string, double> variable_values;
        double objective_value = 0.0;
        bool is_feasible = false;
    };
}