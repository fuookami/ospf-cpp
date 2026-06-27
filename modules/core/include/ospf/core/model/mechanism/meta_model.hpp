#pragma once
/// Mechanism meta model /// 1:1 ∂‘”¶ Rust/core/model/mechanism/meta_model.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct MechanismMetaModel {
        std::string name;
        std::vector<std::string> variable_names;
        std::vector<std::string> constraint_names;
    };
}