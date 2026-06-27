#pragma once
/// Model mechanism /// 1:1 对应 Rust core/model/mechanism.rs
#include <string>
#include <vector>
namespace ospf::core {
    /// 机制 / Mechanism
    struct Mechanism {
        std::string name;
        std::vector<std::string> constraint_names;
        std::vector<std::string> variable_names;
    };
}