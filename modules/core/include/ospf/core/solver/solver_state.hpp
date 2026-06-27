#pragma once
/// Solver state /// 1:1 ∂‘”¶ Rust/core/solver/solver_state.rs
#include <cstdint>
namespace ospf::core {
    enum class SolverState : uint8_t { Idle, Running, Completed, Error };
}