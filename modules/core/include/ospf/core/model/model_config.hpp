#pragma once
/// Model config /// 1:1 ∂‘”¶ Rust core/model/model_config.rs
#include <string>
namespace ospf::core {
    struct ModelConfig {
        std::string name = "unnamed";
        double tolerance = 1e-6;
        int max_iterations = 1000;
    };
}