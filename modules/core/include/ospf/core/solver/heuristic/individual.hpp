#pragma once
/// Heuristic individual /// 1:1 ∂‘”¶ Rust/core/solver/heuristic/individual.rs
#include <vector>
namespace ospf::core {
    struct Individual { std::vector<double> genes; double fitness = 0.0; };
}