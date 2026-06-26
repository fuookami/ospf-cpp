#pragma once
/// BPP3D domain/layer_assignment/service/assignment /// 1:1 对应 Rust bpp3d/domain/layer_assignment/service/assignment.rs
/// ImpreciseAssignment + PreciseAssignment — 赋值变量集合
/// ImpreciseAssignment: RMP 阶段 x[layer] 连续变量
/// PreciseAssignment: Final MILP 阶段 x[bin,layer] 二值变量

#include <ospf/framework/bpp3d/domain/layer_assignment/service/load.hpp>
#include <ospf/framework/bpp3d/domain/block_loading/model.hpp>

#include <cstddef>
#include <optional>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 不精确赋值 / Imprecise assignment
    // 对应 Rust ImpreciseAssignment<V, U>
    // RMP 阶段：x[layer] 连续变量
    // ============================================================================

    /// 不精确赋值 / Imprecise assignment
    /// 对应 Rust ImpreciseAssignment<V, U>
    /// RMP 阶段：x[layer] 连续变量，支持 load 符号注册
    struct ImpreciseAssignment {
        /// 层列表 / Layers
        std::vector<BinLayer> layers;
        /// 列变量上界 / Column variable upper bounds
        std::vector<std::optional<double>> upper_bounds;
        /// 每层载重 load_weight[layer] = sum(x[layer] * weight[layer])
        std::vector<std::size_t> load_weight_symbol_ids;
        /// 每层体积 load_volume[layer] = sum(x[layer] * volume[layer])
        std::vector<std::size_t> load_volume_symbol_ids;
        /// 每层深度 load_depth[layer] = sum(x[layer] * depth[layer])
        std::vector<std::size_t> load_depth_symbol_ids;
        /// 每层需求覆盖 load[layer] = sum(x[layer] * demand_coeff[layer])
        std::vector<std::size_t> load_symbol_ids;

        /// 层数量 / Layer count
        [[nodiscard]] std::size_t layer_count() const noexcept { return layers.size(); }

        /// 是否为空 / Whether empty
        [[nodiscard]] bool empty() const noexcept { return layers.empty(); }
    };

    // ============================================================================
    // 精确赋值 / Precise assignment
    // 对应 Rust PreciseAssignment<V, U>
    // Final MILP 阶段：x[bin,layer] 二值变量
    // ============================================================================

    /// 精确赋值 / Precise assignment
    /// 对应 Rust PreciseAssignment<V, U>
    struct PreciseAssignment {
        /// 容器列表 / Bins
        std::vector<std::size_t> bin_indices;
        /// 层列表 / Layers
        std::vector<BinLayer> layers;
        /// 载重符号
        std::vector<std::size_t> load_weight_symbol_ids;
        /// 体积符号
        std::vector<std::size_t> load_volume_symbol_ids;
        /// 深度符号
        std::vector<std::size_t> load_depth_symbol_ids;

        [[nodiscard]] std::size_t bin_count() const noexcept { return bin_indices.size(); }
        [[nodiscard]] std::size_t layer_count() const noexcept { return layers.size(); }
    };

}  // namespace ospf::framework::bpp3d
