#pragma once
/// Object category /// 1:1 ∂‘”¶ Rust/core/model/basic/object_category.rs
#include <cstdint>
namespace ospf::core {
    enum class ObjectCategory : uint8_t {
        Variable, Constraint, Objective, Intermediate
    };
}