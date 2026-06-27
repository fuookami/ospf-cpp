#pragma once
/// BPP3D domain/item/package_attribute
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class PackageAttribute : uint8_t { Fragile, Stackable, Orientable };
}