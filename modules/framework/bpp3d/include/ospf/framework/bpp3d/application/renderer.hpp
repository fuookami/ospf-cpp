#pragma once
/// BPP3D application/renderer /// 1:1 ∂‘”¶ Rust bpp3d/application/renderer.rs
#include <string>
namespace ospf::framework::bpp3d {
    struct Renderer {
        std::string format = "json";
        bool pretty_print = true;
    };
}