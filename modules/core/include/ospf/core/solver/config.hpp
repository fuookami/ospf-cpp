#pragma once
/// Solver config /// 1:1 ∂‘”¶ Rust/core/solver/config.rs
#include <string>
namespace ospf::core {
    struct SolverConfig {
        std::string solver_name = "auto";
        double time_limit = -1.0;
        double mip_gap = 1e-4;
        int threads = 0;
        bool suppress_output = true;
    };
}