#pragma once
/// BPP3D domain/item/prototype
#include <string>
namespace ospf::framework::bpp3d {
    struct ItemPrototype {
        std::string id;
        double width = 0, height = 0, depth = 0;
        double weight = 0.0;
    };
}