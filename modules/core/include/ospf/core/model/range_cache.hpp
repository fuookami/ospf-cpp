#pragma once
/// Range cache
#include <unordered_map>
#include <cstdint>
namespace ospf::core {
    struct RangeCache {
        std::unordered_map<std::uint64_t, std::pair<double, double>> cache;
        void clear() { cache.clear(); }
    };
}