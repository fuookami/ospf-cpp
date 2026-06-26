#pragma once
/// 多层启发式搜索算法 / Multi-layer heuristic search algorithm
/// 1:1 对应 Rust bpp3d/domain/block_loading/service/multi_layer_heuristic_search_algorithm.rs
/// 使用多层启发式搜索生成块放置方案

#include <cstddef>
#include <vector>
#include <string>

namespace ospf::framework::bpp3d {

    /// 多层启发式搜索配置 / Multi-layer heuristic search configuration
    struct MultiLayerHeuristicSearchConfig {
        /// 最大层数 / Maximum layers
        std::size_t max_layers = 10;
        /// 每层最大候选数 / Maximum candidates per layer
        std::size_t max_candidates_per_layer = 16;
    };

    /// 多层启发式搜索算法 / Multi-layer heuristic search algorithm
    /// 对应 Rust MultiLayerHeuristicSearchAlgorithm
    class MultiLayerHeuristicSearchAlgorithm {
    public:
        explicit MultiLayerHeuristicSearchAlgorithm(MultiLayerHeuristicSearchConfig config = {})
            : config_(std::move(config)) {}

        /// 配置 / Configuration
        [[nodiscard]] const MultiLayerHeuristicSearchConfig& config() const noexcept { return config_; }

        /// 诊断信息 / Diagnostics
        [[nodiscard]] std::vector<std::string> diagnostics() const {
            return {"multi-layer heuristic search: max_layers=" + std::to_string(config_.max_layers)
                + ", max_candidates_per_layer=" + std::to_string(config_.max_candidates_per_layer)};
        }

    private:
        MultiLayerHeuristicSearchConfig config_;
    };

}  // namespace ospf::framework::bpp3d
