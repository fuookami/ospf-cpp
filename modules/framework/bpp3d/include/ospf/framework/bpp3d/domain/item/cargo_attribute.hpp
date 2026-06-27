#pragma once
/// BPP3D domain/item/cargo_attribute
#include <cstdint>
namespace ospf::framework::bpp3d {
    enum class CargoAttribute : uint8_t { Fragile, Heavy, Stackable, Liquid };
}