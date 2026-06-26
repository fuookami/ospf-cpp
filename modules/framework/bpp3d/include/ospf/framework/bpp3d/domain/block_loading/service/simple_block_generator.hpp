#pragma once
/// BPP3D domain/block_loading/service/simple_block_generator /// 1:1 对应 Rust bpp3d/domain/block_loading/service/simple_block_generator.rs
/// SimpleBlockGeneratorConfig + SimpleBlockGenerator

#include <ospf/framework/bpp3d/domain/block_loading/model.hpp>
#include <ospf/framework/bpp3d/domain/item/model/item.hpp>

#include <algorithm>
#include <cstddef>
#include <optional>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 简单块生成器配置 / Simple block generator config
    // 对应 Rust SimpleBlockGeneratorConfig
    // ============================================================================

    /// 简单块生成器配置 / Simple block generator config
    /// 对应 Rust SimpleBlockGeneratorConfig
    struct SimpleBlockGeneratorConfig {
        /// 启用旋转 / Enable rotation
        bool with_rotation = true;
        /// 启用余量 / Enable remainder blocks
        bool with_remainder = false;
        /// 最大竖向堆叠层数 / Maximum vertical stack layers
        std::optional<std::size_t> max_stack_layers;
    };

    // ============================================================================
    // 简单块生成器 / Simple block generator
    // 对应 Rust SimpleBlockGenerator
    // ============================================================================

    /// 简单块生成器 / Simple block generator
    /// 对应 Rust SimpleBlockGenerator
    struct SimpleBlockGenerator {
        /// 配置 / Configuration
        SimpleBlockGeneratorConfig config;

        /// 创建默认生成器 / Create default generator
        /// 对应 Rust SimpleBlockGenerator::default_generator
        [[nodiscard]] static SimpleBlockGenerator default_generator() {
            return SimpleBlockGenerator{SimpleBlockGeneratorConfig{}};
        }

        /// 创建生成器 / Create generator
        /// 对应 Rust SimpleBlockGenerator::new
        [[nodiscard]] static SimpleBlockGenerator create(SimpleBlockGeneratorConfig cfg) {
            return SimpleBlockGenerator{std::move(cfg)};
        }

        /// 生成简单块 / Generate simple blocks
        /// 对应 Rust SimpleBlockGenerator::generate
        /// items: 物品列表, amounts: 每种物品数量, container: 容器尺寸
        /// 返回：生成的简单块列表
        [[nodiscard]] std::vector<SimpleBlock> generate(
            const std::vector<ActualItem>& items,
            const std::vector<std::size_t>& amounts,
            const MetricSize3& container) const
        {
            std::vector<SimpleBlock> blocks;

            for (std::size_t i = 0; i < items.size() && i < amounts.size(); ++i) {
                const auto& item = items[i];
                auto amount = amounts[i];

                // 计算物品尺寸
                double w = item.width, h = item.height, d = item.depth;

                // 尝试不同朝向
                std::vector<std::tuple<double,double,double>> orientations = {{w, h, d}};
                if (config.with_rotation) {
                    orientations.push_back({h, w, d});  // Side
                    orientations.push_back({w, d, h});  // Lie
                }

                for (const auto& [ow, oh, od] : orientations) {
                    std::size_t max_nx = static_cast<std::size_t>(container.width / ow);
                    std::size_t max_ny = static_cast<std::size_t>(container.height / oh);
                    std::size_t max_nz = static_cast<std::size_t>(container.depth / od);

                    if (config.max_stack_layers.has_value()) {
                        max_ny = std::min(max_ny, *config.max_stack_layers);
                    }
                    if (max_nx == 0 || max_ny == 0 || max_nz == 0) continue;

                    std::size_t max_items = max_nx * max_ny * max_nz;
                    std::size_t actual = std::min(amount, max_items);

                    std::size_t nx = std::min(max_nx, static_cast<std::size_t>(std::sqrt(static_cast<double>(actual))));
                    if (nx < 1) nx = 1;
                    std::size_t nz = std::min(nx, max_nz);
                    std::size_t ny = max_ny;

                    while (nx * ny * nz < actual) {
                        if (nx < max_nx) ++nx;
                        else if (nz < max_nz) ++nz;
                        else break;
                    }
                    if (config.max_stack_layers.has_value()) {
                        ny = std::min(ny, *config.max_stack_layers);
                    }

                    if (nx * ny * nz > 0) {
                        ItemView view;
                        view.item_index = i;
                        view.width = ow; view.height = oh; view.depth = od;
                        view.weight = item.weight;
                        blocks.push_back(SimpleBlock::from_item_view(view, nx, ny, nz));
                        break;  // 每种物品只用第一个可行朝向
                    }
                }
            }
            return blocks;
        }
    };

}  // namespace ospf::framework::bpp3d
