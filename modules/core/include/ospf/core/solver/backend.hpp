#pragma once
/// Solver backend /// 1:1 ∂‘”¶ Rust core/solver/backend.rs
#include <string>
#include <optional>
namespace ospf::core {
    struct SolverBackend {
        std::string name;
        bool available = false;
        std::optional<std::string> version;
    };
}