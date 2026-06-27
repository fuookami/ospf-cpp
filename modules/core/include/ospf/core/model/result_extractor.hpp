#pragma once
/// Result extractor /// 1:1 ∂‘”¶ Rust/core/model/result_extractor.rs
#include <string>
#include <unordered_map>
namespace ospf::core {
    struct ResultExtractor {
        std::unordered_map<std::string, double> variable_values;
        double objective_value = 0.0;
        bool is_optimal = false;
    };
}