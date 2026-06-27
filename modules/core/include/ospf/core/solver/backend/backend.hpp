#pragma once
/// Solver backend /// 1:1 ∂‘”¶ Rust core/solver/backend/backend.rs
#include <string>
namespace ospf::core {
    struct SolverBackendConfig {
        std::string name;
        bool available = false;
    };
}