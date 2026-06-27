#pragma once
/// Derived quantity /// 1:1 ∂‘”¶ Rust quantities/dimension/derived_quantity.rs
#include <string>
namespace ospf::quantities {
    struct DerivedQuantity {
        std::string name;
        std::string symbol;
    };
}