#pragma once
/// Basic model
#include <string>
namespace ospf::core {
    struct BasicModel {
        std::string name;
        int variable_count = 0;
        int constraint_count = 0;
    };
}