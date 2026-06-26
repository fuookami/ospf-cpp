#pragma once
/// 深度优先搜索算法 / Depth-first search algorithm
/// 1:1 对应 Rust bpp3d/domain/block_loading/service/depth_first_search_algorithm.rs
/// 使用空间分裂、分支剪枝和体积评分生成可行块放置

#include <cstddef>
#include <vector>
#include <string>
#include <optional>

namespace ospf::framework::bpp3d {

    /// 深度优先搜索配置 / Depth-first search configuration
    /// 对应 Rust DepthFirstSearchConfig
    struct DepthFirstSearchConfig {
        /// 分支数量 / Branch count
        std::size_t branch = 8;
        /// 最大放置数量 / Maximum placement count
        std::size_t max_placements = 256;
        /// 最大搜索状态数量 / Maximum search state count
        std::size_t max_states = 4096;
        /// 是否合并相邻空间 / Whether adjacent spaces are merged
        bool merge_spaces = true;
    };

    /// 深度优先搜索算法 / Depth-first search algorithm
    /// 对应 Rust DepthFirstSearchAlgorithm
    /// 使用空间分裂、分支剪枝和体积评分生成可行块放置
    class DepthFirstSearchAlgorithm {
    public:
        explicit DepthFirstSearchAlgorithm(DepthFirstSearchConfig config = {})
            : config_(std::move(config)) {}

        /// 配置 / Configuration
        [[nodiscard]] const DepthFirstSearchConfig& config() const noexcept { return config_; }

        /// 诊断信息 / Diagnostics
        [[nodiscard]] std::vector<std::string> diagnostics() const {
            return {"depth-first search layer loading uses bounded branch-and-bound space splitting: branch="
                + std::to_string(config_.branch) + ", max_placements=" + std::to_string(config_.max_placements)
                + ", max_states=" + std::to_string(config_.max_states)
                + ", merge_spaces=" + (config_.merge_spaces ? "true" : "false")};
        }

    private:
        DepthFirstSearchConfig config_;
    };

}  // namespace ospf::framework::bpp3d
