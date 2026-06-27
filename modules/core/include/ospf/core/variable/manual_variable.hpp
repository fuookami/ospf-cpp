#pragma once
/// Manual variable
#include <ospf/core/variable.hpp>
namespace ospf::core {
    struct ManualVariable {
        Variable var;
        bool user_defined = true;
    };
}