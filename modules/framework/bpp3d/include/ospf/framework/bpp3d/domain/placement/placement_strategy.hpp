#pragma once
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class PlacementStrategy : uint8_t { BottomLeft, BestFit, FirstFitDecreasing };
}