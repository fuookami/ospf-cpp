#pragma once
/// Solver ext /// 1:1 ∂‘”¶ Rust/core/solver/solver_ext.rs
#include <string>
namespace ospf::core {
    struct SolverExt { bool supports_mip = true; bool supports_qp = false; };
}