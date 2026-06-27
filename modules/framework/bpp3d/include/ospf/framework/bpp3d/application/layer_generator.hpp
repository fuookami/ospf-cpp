#pragma once
/// BPP3D application/layer_generator /// 1:1 ∂‘”¶ Rust bpp3d/application/layer_generator.rs
#include <string>
#include <vector>
namespace ospf::framework::bpp3d {
    struct LayerGenerator {
        std::string name = "DefaultLayerGenerator";
        int max_layers = 100;
        double max_height = 0.0;
    };
}