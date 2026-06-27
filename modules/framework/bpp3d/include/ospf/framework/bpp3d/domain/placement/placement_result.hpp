#pragma once
#include <string>
namespace ospf::framework::bpp3d {
    struct PlacementResult {
        bool success = false;
        double x = 0, y = 0, z = 0;
        int rotation = 0;
    };
}