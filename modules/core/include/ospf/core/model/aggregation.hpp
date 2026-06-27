#pragma once
/// Model aggregation
#include <string>
#include <vector>
namespace ospf::core {
    struct ModelAggregation {
        std::string name;
        std::vector<std::string> model_names;
    };
}