#pragma once
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class LayerPattern : uint8_t { Row, Column, Staircase, Custom };
}