#pragma once
/// IIS model /// 1:1 ∂‘”¶ Rust/core/solver/iis/iis_model.rs
#include <vector>
#include <string>
namespace ospf::core {
    struct IisModel {
        std::vector<std::string> constraints;
        std::vector<std::string> variables;
    };
}