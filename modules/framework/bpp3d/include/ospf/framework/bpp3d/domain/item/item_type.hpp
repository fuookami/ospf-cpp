#pragma once
/// BPP3D domain/item/item_type
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class ItemType : uint8_t { Cuboid, Cylinder, Irregular };
}