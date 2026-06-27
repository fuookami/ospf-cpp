#pragma once
/// Gurobi linear solver /// 1:1 ∂‘”¶ Rust/core/solver/solvers/gurobi/linear.rs
#ifdef OSPF_ENABLE_GUROBI
#include <ospf/core/solver/solvers/gurobi/solver.hpp>
namespace ospf::core {
    using GurobiLinearSolver = GurobiSolver;
}
#endif