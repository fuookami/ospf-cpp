#pragma once
/// Constraint priority /// 1:1 ∂‘”¶ Rust/core/model/basic/constraint_priority.rs
#include <cstdint>
namespace ospf::core {
    enum class ConstraintPriority : uint8_t {
        Low = 0, Normal = 1, High = 2, Critical = 3
    };
}