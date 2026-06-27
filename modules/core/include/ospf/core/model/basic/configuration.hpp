#pragma once
/// Model configuration /// 1:1 ∂‘”¶ Rust/core/model/basic/configuration.rs
#include <string>
namespace ospf::core {
    struct ModelConfiguration {
        std::string name;
        double tolerance = 1e-6;
        int max_iterations = 1000;
    };
}