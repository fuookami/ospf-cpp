#pragma once
/// Mechanism constraint /// 1:1 ∂‘”¶ Rust/core/model/mechanism/constraint.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct MechanismConstraint {
        std::string name;
        std::vector<std::pair<std::size_t, double>> coefficients;
        double rhs = 0.0;
    };
}