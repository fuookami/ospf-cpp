#pragma once
/// Quantities error /// 1:1 ∂‘”¶ Rust quantities/error.rs
#include <string>
namespace ospf::quantities {
    struct QuantityError {
        std::string message;
        std::string code;
    };
}