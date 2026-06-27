#pragma once
/// Heuristic algorithm /// 1:1 ∂‘”¶ Rust/core/solver/heuristic/algorithm.rs
#include <string>
namespace ospf::core {
    struct HeuristicAlgorithm {
        std::string name = "auto";
        int max_iterations = 100;
        double time_limit = 10.0;
    };
}