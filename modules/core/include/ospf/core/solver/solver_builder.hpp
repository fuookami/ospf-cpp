#pragma once
/// Solver builder /// 1:1 ∂‘”¶ Rust/core/solver/solver_builder.rs
#include <string>
namespace ospf::core {
    struct SolverBuilder {
        std::string solver_name = "auto";
        SolverBuilder& set_name(const std::string& n) { solver_name = n; return *this; }
    };
}