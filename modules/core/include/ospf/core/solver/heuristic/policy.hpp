#pragma once
/// Heuristic policy /// 1:1 ∂‘”¶ Rust/core/solver/heuristic/policy.rs
#include <cstdint>
namespace ospf::core {
    enum class SelectionPolicy : uint8_t { Tournament, Roulette, Rank };
}