#pragma once
/// Mechanism model /// 1:1 ∂‘”¶ Rust/core/model/mechanism/mechanism_model.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct MechanismModel {
        std::string name;
        std::vector<std::string> mechanism_names;
    };
}