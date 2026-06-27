#pragma once
/// BPP3D application/algorithm/policy /// 1:1 ∂‘”¶ Rust bpp3d/application/algorithm/policy.rs
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class PackingPolicy : uint8_t {
        FirstFitDecreasing,
        BestFitDecreasing,
        ColumnGeneration,
    };
}