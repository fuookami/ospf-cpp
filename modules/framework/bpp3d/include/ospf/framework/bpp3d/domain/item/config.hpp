#pragma once
/// BPP3D domain/item/config
namespace ospf::framework::bpp3d {
    struct ItemConfig {
        bool allow_rotation = true;
        bool allow_stacking = true;
        double max_stack_weight = 1000.0;
    };
}