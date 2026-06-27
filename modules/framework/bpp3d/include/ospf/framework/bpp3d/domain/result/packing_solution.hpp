#pragma once
#include <vector>
namespace ospf::framework::bpp3d {
    struct PackingSolution {
        int bin_count = 0;
        double total_volume = 0.0;
        double utilization = 0.0;
        std::vector<int> bin_assignments;
    };
}