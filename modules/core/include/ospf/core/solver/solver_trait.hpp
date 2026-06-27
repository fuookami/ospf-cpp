#pragma once
/// Solver trait /// 1:1 ∂‘”¶ Rust/core/solver/solver_trait.rs
#include <ospf/core/solver/solve_result.hpp>
namespace ospf::core {
    template<typename T>
    concept SolverTrait = requires(T t) {
        { t.name() };
    };
}