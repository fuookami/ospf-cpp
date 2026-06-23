#pragma once

/// BPP3D 领域模型 / BPP3D domain model
/// 3D 装箱问题的领域类型：Bin、Item、Layer。

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

namespace ospf::framework::bpp3d {

    /// 3D 尺寸 / 3D dimensions
    struct Dimensions {
        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;

        [[nodiscard]] double volume() const noexcept { return width * height * depth; }
    };

    /// 3D 位置 / 3D position
    struct Position {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    };

    /// 物品 / Item
    struct Item {
        std::string id;
        Dimensions dimensions;
        double weight = 0.0;

        [[nodiscard]] double volume() const noexcept { return dimensions.volume(); }
    };

    /// 容器（Bin） / Container (Bin)
    struct Bin {
        std::string id;
        Dimensions dimensions;
        double max_weight = 0.0;

        [[nodiscard]] double volume() const noexcept { return dimensions.volume(); }
    };

    /// 放置的物品 / Placed item
    struct PlacedItem {
        Item item;
        Position position;

        [[nodiscard]] bool overlaps(const PlacedItem& other) const {
            // 检查两个物品是否重叠
            bool x_overlap = position.x < other.position.x + other.item.dimensions.width &&
                             position.x + item.dimensions.width > other.position.x;
            bool y_overlap = position.y < other.position.y + other.item.dimensions.height &&
                             position.y + item.dimensions.height > other.position.y;
            bool z_overlap = position.z < other.position.z + other.item.dimensions.depth &&
                             position.z + item.dimensions.depth > other.position.z;
            return x_overlap && y_overlap && z_overlap;
        }
    };

    /// 层 / Layer
    struct Layer {
        double height = 0.0;
        std::vector<PlacedItem> items;

        [[nodiscard]] double used_area() const noexcept {
            double area = 0.0;
            for (const auto& item : items) {
                area += item.item.dimensions.width * item.item.dimensions.depth;
            }
            return area;
        }
    };

    /// 装箱解决方案 / Packing solution
    struct PackingSolution {
        std::string bin_id;
        std::vector<PlacedItem> placed_items;
        std::vector<Layer> layers;

        [[nodiscard]] double total_volume() const noexcept {
            double vol = 0.0;
            for (const auto& item : placed_items) {
                vol += item.item.volume();
            }
            return vol;
        }

        [[nodiscard]] double utilization(double bin_volume) const noexcept {
            if (bin_volume <= 0.0) return 0.0;
            return total_volume() / bin_volume;
        }
    };

}  // namespace ospf::framework::bpp3d
