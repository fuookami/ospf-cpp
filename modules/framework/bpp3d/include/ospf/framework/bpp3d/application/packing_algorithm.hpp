#pragma once
/// BPP3D application/packing_algorithm /// 1:1 ∂‘”¶ Rust bpp3d/application/packing_algorithm.rs
#include <string>
#include <vector>
namespace ospf::framework::bpp3d {
    struct PackingAlgorithm {
        std::string name = "FirstFitDecreasing";
        int max_iterations = 100;
        double tolerance = 1e-6;
    };
}