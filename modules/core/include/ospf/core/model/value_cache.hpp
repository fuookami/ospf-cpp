#pragma once
/// Value cache
#include <unordered_map>
#include <cstdint>
namespace ospf::core {
    struct ValueCache {
        std::unordered_map<std::uint64_t, double> cache;
        void clear() { cache.clear(); }
    };
}