#pragma once
/// Variable ID
#include <cstdint>
#include <string>
namespace ospf::core {
    struct VariableId {
        std::uint64_t id = 0;
        std::string name;
    };
}