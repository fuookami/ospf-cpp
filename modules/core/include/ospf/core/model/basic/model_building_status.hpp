#pragma once
/// Model building status /// 1:1 ∂‘”¶ Rust/core/model/basic/model_building_status.rs
#include <cstdint>
namespace ospf::core {
    enum class ModelBuildingStatus : uint8_t {
        NotStarted, InProgress, Completed, Failed
    };
}