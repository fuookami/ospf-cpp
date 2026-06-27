#pragma once
/// Constraint group /// 1:1 ∂‘”¶ Rust/core/model/mechanism/constraint_group.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ConstraintGroup {
        std::string name;
        std::vector<std::string> constraint_names;
    };
}