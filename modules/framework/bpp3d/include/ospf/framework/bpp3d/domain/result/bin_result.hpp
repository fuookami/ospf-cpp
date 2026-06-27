#pragma once
namespace ospf::framework::bpp3d {
    struct BinResult {
        int bin_index = 0;
        double used_volume = 0.0;
        double utilization = 0.0;
        int item_count = 0;
    };
}