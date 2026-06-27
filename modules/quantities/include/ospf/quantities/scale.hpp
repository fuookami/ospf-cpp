#pragma once
/// Quantities scale /// 1:1 ∂‘”¶ Rust quantities/scale.rs
#include <string>
namespace ospf::quantities {
    struct Scale {
        std::string name;
        double factor = 1.0;
    };
}