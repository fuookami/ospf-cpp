#pragma once
/// Conversion context /// 1:1 ∂‘”¶ Rust/core/solver/value/conversion_context.rs
#include <string>
namespace ospf::core {
    struct ConversionContext { std::string source_unit; std::string target_unit; };
}