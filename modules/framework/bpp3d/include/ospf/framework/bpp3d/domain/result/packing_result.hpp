#pragma once
#include <string>
#include <vector>
namespace ospf::framework::bpp3d {
    struct PackingResult {
        int bin_count = 0;
        double total_volume = 0.0;
        double utilization = 0.0;
        std::vector<std::string> packed_item_ids;
    };
}