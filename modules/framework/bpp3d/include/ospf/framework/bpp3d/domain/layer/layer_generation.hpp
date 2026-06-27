#pragma once
#include <vector>
namespace ospf::framework::bpp3d {
    struct LayerGenerationRequest {
        double width = 0.0;
        double depth = 0.0;
        int max_items = 100;
    };
}