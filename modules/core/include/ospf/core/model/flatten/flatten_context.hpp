#pragma once
/// Flatten context /// 1:1 ∂‘”¶ Rust/core/model/flatten/flatten_context.rs
#include <string>
#include <unordered_map>
namespace ospf::core {
    struct FlattenContext {
        std::unordered_map<std::size_t, double> variable_values;
        int depth = 0;
    };
}