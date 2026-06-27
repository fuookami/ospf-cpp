#pragma once
/// Physical unit /// 1:1 ∂‘”¶ Rust quantities/unit/physical_unit.rs
#include <ospf/quantities/unit.hpp>
#include <string>
namespace ospf::quantities {
    struct PhysicalUnit {
        Dimension dimension;
        double to_si_factor = 1.0;
        std::string name;
        std::string symbol;
    };
}