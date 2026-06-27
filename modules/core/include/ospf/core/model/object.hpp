#pragma once
/// Model object
#include <string>
namespace ospf::core {
    struct ModelObject {
        std::string name;
        std::string type;
    };
}