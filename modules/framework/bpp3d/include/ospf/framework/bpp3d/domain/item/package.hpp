#pragma once
/// BPP3D domain/item/package
#include <string>
#include <vector>
namespace ospf::framework::bpp3d {
    struct Package {
        std::string id;
        double weight = 0.0;
        std::vector<std::string> item_ids;
    };
}