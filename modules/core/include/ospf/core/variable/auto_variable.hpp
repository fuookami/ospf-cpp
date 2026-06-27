#pragma once
/// Auto variable
#include <ospf/core/variable.hpp>
namespace ospf::core {
    struct AutoVariable {
        Variable var;
        bool auto_generated = true;
    };
}