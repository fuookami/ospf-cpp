#pragma once
/// BPP3D domain/item/component
#include <string>
namespace ospf::framework::bpp3d {
    struct ItemComponent {
        std::string id;
        double weight = 0.0;
    };
}