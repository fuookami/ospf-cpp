#pragma once
/// BPP3D domain/item/stowage
namespace ospf::framework::bpp3d {
    struct StowageInfo {
        int bin_index = -1;
        double x = 0, y = 0, z = 0;
        int rotation = 0;
    };
}