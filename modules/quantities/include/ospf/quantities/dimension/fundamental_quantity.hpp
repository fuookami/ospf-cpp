#pragma once
/// Fundamental quantity /// 1:1 ∂‘”¶ Rust quantities/dimension/fundamental_quantity.rs
#include <string>
namespace ospf::quantities {
    struct FundamentalQuantity {
        std::string name;
        std::string symbol;
    };
}