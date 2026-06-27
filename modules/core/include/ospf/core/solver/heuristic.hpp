#pragma once
/// Heuristic /// 1:1 ∂‘”¶ Rust/core/solver/heuristic.rs
#include <string>
namespace ospf::core {
    struct HeuristicConfig {
        std::string type = "auto";
        double time_limit = 10.0;
        int max_iterations = 100;
    };
}