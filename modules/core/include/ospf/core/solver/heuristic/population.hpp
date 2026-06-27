#pragma once
/// Heuristic population /// 1:1 ∂‘”¶ Rust/core/solver/heuristic/population.rs
#include <vector>
#include "individual.hpp"
namespace ospf::core {
    struct Population { std::vector<Individual> individuals; int size() const { return individuals.size(); } };
}