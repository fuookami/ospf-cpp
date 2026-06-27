#pragma once
/// IIS /// 1:1 ∂‘”¶ Rust/core/solver/iis/iis.rs
#include <vector>
#include <string>
namespace ospf::core {
    struct IisResult {
        bool found = false;
        std::vector<std::string> constraint_names;
        std::vector<std::string> variable_names;
    };
}