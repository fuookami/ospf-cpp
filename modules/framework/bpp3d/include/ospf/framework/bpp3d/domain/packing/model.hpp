#pragma once
/// BPP3D domain/packing/model /// 1:1 对应 Rust bpp3d/domain/packing/model.rs
/// PackedItem + PackedBin + PackingResult + Packer + MaterialSummary

#include <ospf/framework/bpp3d/infrastructure/packing_shape.hpp>
#include <ospf/framework/bpp3d/infrastructure/orientation.hpp>
#include <ospf/framework/bpp3d/domain/item/model/material.hpp>

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 装箱物品 / Packed item
    // 对应 Rust PackedItem<V, U>
    // ============================================================================

    /// 装箱物品 / Packed item
    /// 对应 Rust PackedItem<V, U>
    struct PackedItem {
        /// 原始物品索引 / Original item index
        std::size_t item_index = 0;
        /// 物品宽度 / Item width
        double width = 0.0;
        /// 物品高度 / Item height
        double height = 0.0;
        /// 物品深度 / Item depth
        double depth = 0.0;
        /// 物品重量 / Item weight
        double weight = 0.0;
        /// 放置位置 / Placement position
        double x = 0.0, y = 0.0, z = 0.0;
        /// 朝向 / Orientation
        Orientation orientation = Orientation::Upright;
        /// 装载顺序 / Loading order
        std::size_t loading_order = 0;

        /// 实际体积 / Actual volume
        [[nodiscard]] double actual_volume() const noexcept {
            return width * height * depth;
        }
    };

    // ============================================================================
    // 装箱容器 / Packed bin
    // 对应 Rust PackedBin<V, U>
    // ============================================================================

    /// 装箱容器 / Packed bin
    /// 对应 Rust PackedBin<V, U>
    struct PackedBin {
        /// 容器名 / Bin name
        std::string name;
        /// 容器宽度 / Bin width
        double bin_width = 0.0;
        /// 容器高度 / Bin height
        double bin_height = 0.0;
        /// 容器深度 / Bin depth
        double bin_depth = 0.0;
        /// 容器最大重量 / Bin max weight
        double bin_max_weight = 0.0;
        /// 批次号 / Batch number
        std::optional<std::size_t> batch_no;
        /// 装入物品 / Packed items
        std::vector<PackedItem> items;

        /// 总重量 / Total weight
        [[nodiscard]] double total_weight() const noexcept {
            double sum = 0.0;
            for (const auto& item : items) sum += item.weight;
            return sum;
        }

        /// 总实际体积 / Total actual volume
        [[nodiscard]] double total_actual_volume() const noexcept {
            double sum = 0.0;
            for (const auto& item : items) sum += item.actual_volume();
            return sum;
        }
    };

    // ============================================================================
    // 物料汇总 / Material summary
    // 对应 Rust MaterialSummary
    // ============================================================================

    /// 物料汇总 / Material summary
    /// 对应 Rust MaterialSummary
    struct MaterialSummary {
        /// 物料标识 / Material key
        MaterialKey material;
        /// 数量 / Amount
        std::size_t amount = 0;
    };

    // ============================================================================
    // 装箱结果 / Packing result
    // 对应 Rust PackingResult<V, U>
    // ============================================================================

    /// 装箱结果 / Packing result
    /// 对应 Rust PackingResult<V, U>
    struct PackingResult {
        /// 装箱容器 / Packed bins
        std::vector<PackedBin> packed_bins;
        /// 物料汇总 / Material summaries
        std::vector<MaterialSummary> material_summaries;
    };

    // ============================================================================
    // 装箱器 / Packer
    // 对应 Rust Packer
    // ============================================================================

    /// 装箱器 / Packer
    /// 对应 Rust Packer
    struct Packer {
        /// 装箱 / Invoke packer
        /// 对应 Rust Packer::invoke
        [[nodiscard]] PackingResult invoke(std::vector<PackedBin> bins) const {
            PackingResult result;
            result.packed_bins = std::move(bins);

            // 计算物料汇总 / Compute material summaries
            for (const auto& bin : result.packed_bins) {
                for (const auto& item : bin.items) {
                    MaterialKey key{"DEFAULT", MaterialType::RawMaterial};
                    bool found = false;
                    for (auto& summary : result.material_summaries) {
                        if (summary.material == key) {
                            summary.amount += 1;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        result.material_summaries.push_back({key, 1});
                    }
                }
            }

            return result;
        }
    };

}  // namespace ospf::framework::bpp3d
