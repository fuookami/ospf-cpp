#pragma once
/// Meta constraint /// 1:1 ∂‘”¶ Rust/core/model/mechanism/meta_constraint.rs
#include <string>
namespace ospf::core {
    struct MetaConstraint {
        std::string name;
        std::string expression;
    };
}