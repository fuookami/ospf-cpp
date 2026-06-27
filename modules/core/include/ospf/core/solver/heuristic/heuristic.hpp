#pragma once
/// Heuristic /// 1:1 ∂‘”¶ Rust/core/solver/heuristic/heuristic.rs
#include <string>
namespace ospf::core {
    struct HeuristicSolver {
        std::string name = "auto";
        int max_iterations = 100;
    };
}