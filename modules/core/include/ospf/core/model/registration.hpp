#pragma once
/// Model registration
#include <string>
namespace ospf::core {
    struct ModelRegistration {
        std::string name;
        bool registered = false;
    };
}