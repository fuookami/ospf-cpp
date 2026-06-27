#pragma once
/// Basic mechanism model /// 1:1 ∂‘”¶ Rust/core/model/mechanism/basic_mechanism_model.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct BasicMechanismModel {
        std::string name;
        std::vector<std::string> constraint_names;
    };
}