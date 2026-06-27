#pragma once
/// BPP3D domain/layer/layer
#include <string>
#include <vector>
namespace ospf::framework::bpp3d {
    struct Layer {
        int index = 0;
        double height = 0.0;
        double max_height = 0.0;
        std::vector<int> item_indices;
        [[nodiscard]] double remaining_height() const { return max_height - height; }
    };
}