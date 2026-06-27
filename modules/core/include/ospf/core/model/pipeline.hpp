#pragma once
/// Model pipeline
#include <string>
#include <vector>
namespace ospf::core {
    struct ModelPipeline {
        std::string name;
        std::vector<std::string> stages;
    };
}