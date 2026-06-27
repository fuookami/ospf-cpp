#pragma once
namespace ospf::framework::bpp3d {
    struct PlacementValidator {
        bool check_overlap = true;
        bool check_bounds = true;
        bool check_weight = true;
    };
}