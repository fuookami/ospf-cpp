#pragma once
namespace ospf::framework::bpp3d {
    struct ItemResult {
        int item_index = 0;
        int bin_index = -1;
        double x = 0, y = 0, z = 0;
        bool packed = false;
    };
}