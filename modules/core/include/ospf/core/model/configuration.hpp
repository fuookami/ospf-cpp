#pragma once
/// Model configuration
#include <string>
namespace ospf::core {
    struct ModelConfig {
        std::string name;
        double tolerance = 1e-6;
        int max_iterations = 1000;
    };
}