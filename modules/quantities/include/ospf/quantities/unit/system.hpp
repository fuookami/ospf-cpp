#pragma once
/// Unit system /// 1:1 ∂‘”¶ Rust quantities/unit/system.rs
#include <string>
#include <vector>
namespace ospf::quantities {
    struct UnitSystem {
        std::string name;
        std::vector<std::string> unit_names;
    };
}