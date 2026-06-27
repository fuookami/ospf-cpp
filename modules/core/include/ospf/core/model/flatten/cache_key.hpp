#pragma once
/// Flatten cache key /// 1:1 ∂‘”¶ Rust/core/model/flatten/cache_key.rs
#include <cstdint>
namespace ospf::core {
    struct FlattenCacheKey {
        std::uint64_t symbol_id = 0;
        [[nodiscard]] bool operator==(const FlattenCacheKey&) const = default;
    };
}