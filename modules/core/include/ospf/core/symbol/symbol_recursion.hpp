#pragma once
/// Symbol recursion /// 1:1 ∂‘”¶ Rust core/symbol/symbol_recursion.rs
#include <cstdint>
namespace ospf::core {
    struct RecursionGuard { int depth = 0; int max_depth = 100; };
}