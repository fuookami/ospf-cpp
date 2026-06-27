#pragma once
/// Conversion policy /// 1:1 ∂‘”¶ Rust/core/solver/value/conversion_policy.rs
#include <cstdint>
namespace ospf::core {
    enum class ConversionPolicy : uint8_t { Strict, Lenient, Auto };
}