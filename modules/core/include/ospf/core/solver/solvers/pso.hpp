#pragma once
/// PSO solver /// 1:1 ∂‘”¶ Rust/core/solver/solvers/pso.rs
#include <string>
namespace ospf::core {
    struct PsoConfig { int swarm_size = 50; int max_iterations = 100; double inertia = 0.7; };
}