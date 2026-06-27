#pragma once
/// Gurobi quadratic solver /// 1:1 ∂‘”¶ Rust/core/solver/solvers/gurobi/quadratic.rs
#ifdef OSPF_ENABLE_GUROBI
#include <ospf/core/solver/solvers/gurobi/solver.hpp>
namespace ospf::core {
    using GurobiQuadraticSolver = GurobiSolver;
}
#endif