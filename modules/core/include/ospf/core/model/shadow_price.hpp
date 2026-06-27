#pragma once
/// Shadow price /// 1:1 ∂‘”¶ Rust/core/model/shadow_price.rs
#include <string>
#include <vector>
namespace ospf::core {
    struct ShadowPrice {
        std::string constraint_name;
        double price = 0.0;
    };
}