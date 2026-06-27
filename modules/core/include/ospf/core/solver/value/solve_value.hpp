#pragma once
/// Solve value /// 1:1 ∂‘”¶ Rust/core/solver/value/solve_value.rs
#include <optional>
namespace ospf::core {
    struct SolveValue {
        double value = 0.0;
        bool is_integer = false;
        std::optional<double> lower_bound;
        std::optional<double> upper_bound;
    };
}