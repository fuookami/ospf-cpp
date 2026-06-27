#pragma once
/// Model state /// 1:1 ∂‘”¶ Rust core/model/model_state.rs
#include <cstdint>
namespace ospf::core {
    enum class ModelState : uint8_t {
        Created, Building, Solving, Solved, Error
    };
}