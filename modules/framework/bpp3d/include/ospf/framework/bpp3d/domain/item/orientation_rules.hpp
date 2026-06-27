#pragma once
/// BPP3D domain/item/orientation_rules
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class OrientationRule : uint8_t { Any, Fixed, Limited };
}