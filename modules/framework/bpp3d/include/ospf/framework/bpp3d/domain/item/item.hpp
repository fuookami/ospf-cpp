#pragma once
/// BPP3D domain/item/item /// 1:1 对应 Rust bpp3d/domain/item/item.rs
#include <string>
namespace ospf::framework::bpp3d {
    /// 物品 / Item
    struct Item {
        std::string id;
        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;
        double weight = 0.0;
        int quantity = 1;
        [[nodiscard]] double volume() const { return width * height * depth; }
    };
}