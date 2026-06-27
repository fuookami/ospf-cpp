#pragma once
/// Gurobi config /// 1:1 ∂‘”¶ Rust/core/solver/solvers/gurobi/config.rs
#include <string>
namespace ospf::core {
    struct GurobiConfig {
        bool suppress_output = true;
        double time_limit = -1.0;
        double mip_gap = 1e-4;
        int threads = 0;
    };
}