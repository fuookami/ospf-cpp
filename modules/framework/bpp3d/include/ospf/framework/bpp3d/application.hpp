#pragma once

/// BPP3D 应用层 / BPP3D application layer
/// 打包算法和层生成器。

#include <ospf/framework/bpp3d/domain.hpp>
#include <algorithm>
#include <vector>

namespace ospf::framework::bpp3d {

    /// 打包算法基类 / Packing algorithm base class
    class PackingAlgorithm {
    public:
        virtual ~PackingAlgorithm() = default;

        /// 执行打包 / Execute packing
        [[nodiscard]] virtual PackingSolution pack(
            const Bin& bin, const std::vector<Item>& items) = 0;

        /// 算法名称 / Algorithm name
        [[nodiscard]] virtual const std::string& name() const = 0;
    };

    /// 首次适应递减算法 / First Fit Decreasing algorithm
    class FirstFitDecreasing : public PackingAlgorithm {
    public:
        [[nodiscard]] PackingSolution pack(
            const Bin& bin, const std::vector<Item>& items) override
        {
            PackingSolution solution;
            solution.bin_id = bin.id;

            // 按体积降序排序
            std::vector<Item> sorted_items = items;
            std::sort(sorted_items.begin(), sorted_items.end(),
                [](const Item& a, const Item& b) {
                    return a.volume() > b.volume();
                });

            // 简单的贪心放置
            double current_x = 0.0;
            double current_y = 0.0;
            double current_z = 0.0;
            double max_height_in_row = 0.0;

            for (const auto& item : sorted_items) {
                // 尝试在当前行放置
                if (current_x + item.dimensions.width <= bin.dimensions.width) {
                    PlacedItem placed;
                    placed.item = item;
                    placed.position = {current_x, current_y, current_z};
                    solution.placed_items.push_back(placed);

                    current_x += item.dimensions.width;
                    max_height_in_row = std::max(max_height_in_row, item.dimensions.height);
                } else {
                    // 换行
                    current_x = 0.0;
                    current_y += max_height_in_row;
                    max_height_in_row = 0.0;

                    // 检查是否能放入新行
                    if (current_y + item.dimensions.height <= bin.dimensions.height &&
                        item.dimensions.width <= bin.dimensions.width) {
                        PlacedItem placed;
                        placed.item = item;
                        placed.position = {current_x, current_y, current_z};
                        solution.placed_items.push_back(placed);

                        current_x += item.dimensions.width;
                        max_height_in_row = item.dimensions.height;
                    }
                    // 否则跳过该物品
                }
            }

            return solution;
        }

        [[nodiscard]] const std::string& name() const override {
            static const std::string n = "FirstFitDecreasing";
            return n;
        }
    };

    /// 层生成器 / Layer generator
    class LayerGenerator {
    public:
        /// 从放置的物品生成层 / Generate layers from placed items
        [[nodiscard]] static std::vector<Layer> generate_layers(
            const std::vector<PlacedItem>& items)
        {
            std::vector<Layer> layers;

            // 按 z 坐标分组
            for (const auto& item : items) {
                bool placed = false;
                for (auto& layer : layers) {
                    if (std::abs(layer.height - item.position.z) < 1e-10) {
                        layer.items.push_back(item);
                        placed = true;
                        break;
                    }
                }
                if (!placed) {
                    Layer layer;
                    layer.height = item.position.z;
                    layer.items.push_back(item);
                    layers.push_back(layer);
                }
            }

            return layers;
        }
    };

}  // namespace ospf::framework::bpp3d
