#pragma once
/// Model context
#include <string>
namespace ospf::core {
    struct ModelContext {
        std::string model_name;
        int iteration = 0;
    };
}