#pragma once
/// SCIP config /// 1:1 ∂‘”¶ Rust/core/solver/solvers/scip/config.rs
#include <string>
namespace ospf::core {
    struct ScipConfig {
        bool suppress_output = true;
        double time_limit = -1.0;
    };
}