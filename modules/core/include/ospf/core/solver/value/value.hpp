#pragma once
/// Solver value /// 1:1 ∂‘”¶ Rust/core/solver/value/value.rs
#include <optional>
namespace ospf::core {
    struct SolverValue {
        double value = 0.0;
        bool is_integer = false;
        std::optional<double> lower;
        std::optional<double> upper;
    };
}