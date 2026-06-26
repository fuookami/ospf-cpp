#pragma once
/// BPP3D domain/block_loading/model /// 1:1 对应 Rust bpp3d/domain/block_loading/model.rs
/// ItemView + SimpleBlock + BlockPlacement + Space (fits/place_block)

#include <cstddef>
#include <string>
#include <vector>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 朝向 / Orientation（复用 package_attribute 中的定义）
    // ============================================================================

    // forward declaration — 实际定义在 attribute.hpp
    enum class Orientation;

    // ============================================================================
    // 物品视图 / Item view
    // 对应 Rust ItemView<V, U>
    // ============================================================================

    /// 物品视图 / Item view
    /// 物品在特定朝向下的尺寸视图，用于块生成
    struct ItemView {
        /// 原始物品索引 / Original item index
        std::size_t item_index = 0;
        /// 朝向 / Orientation
        int orientation = 0;  // 0=Upright, 1=Side, 2=Lie
        /// 视图宽度 / View width
        double width = 0.0;
        /// 视图高度 / View height
        double height = 0.0;
        /// 视图深度 / View depth
        double depth = 0.0;
        /// 重量 / Weight
        double weight = 0.0;
    };

    // ============================================================================
    // 度量点 / Metric point 3D
    // 对应 Rust MetricPoint3<V, U>
    // ============================================================================

    /// 3D 度量点 / Metric point 3D
    struct MetricPoint3 {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
    };

    // ============================================================================
    // 度量尺寸 / Metric size 3D
    // 对应 Rust MetricSize3<V, U>
    // ============================================================================

    /// 3D 度量尺寸 / Metric size 3D
    struct MetricSize3 {
        double width = 0.0;
        double height = 0.0;
        double depth = 0.0;
    };

    // ============================================================================
    // 简单块 / Simple block（前向声明，Space 需要引用）
    // ============================================================================

    struct SimpleBlock;

    // ============================================================================
    // 空间 / Space
    // 对应 Rust Space<V, U>
    // ============================================================================

    /// 空间 / Space
    /// 表示容器中的可用空间
    struct Space {
        /// 原点 / Origin
        MetricPoint3 origin;
        /// 尺寸 / Size
        MetricSize3 size;

        /// 构造 / Constructor
        Space() = default;
        Space(MetricPoint3 o, MetricSize3 s) : origin(o), size(s) {}

        /// 创建空间 / Create space
        [[nodiscard]] static Space create(MetricPoint3 origin, MetricSize3 size) {
            return Space{origin, size};
        }

        /// 空间宽度 / Space width
        [[nodiscard]] double width() const noexcept { return size.width; }
        /// 空间高度 / Space height
        [[nodiscard]] double height() const noexcept { return size.height; }
        /// 空间深度 / Space depth
        [[nodiscard]] double depth() const noexcept { return size.depth; }

        /// 判断块是否适合空间 / Check whether block fits in space
        /// 对应 Rust Space::fits
        [[nodiscard]] bool fits(const SimpleBlock& block) const noexcept;

        /// 放置块后产生子空间 / Place block and produce sub-spaces
        /// 对应 Rust Space::place_block
        /// 产生右侧、上方、前方三个子空间（跳过零尺寸）
        [[nodiscard]] std::vector<Space> place_block(const SimpleBlock& block) const;
    };

    // ============================================================================
    // 简单块 / Simple block
    // 对应 Rust SimpleBlock<V, U>
    // ============================================================================

    /// 简单块 / Simple block
    /// 由同一物品同一朝向组成的均匀块
    struct SimpleBlock {
        /// 物品视图 / Item view
        ItemView item_view;
        /// X 方向数量 / X direction count
        std::size_t nx = 1;
        /// Y 方向数量 / Y direction count
        std::size_t ny = 1;
        /// Z 方向数量 / Z direction count
        std::size_t nz = 1;
        /// 总物品数 / Total item count
        std::size_t item_count = 1;
        /// 块宽度 / Block width
        double width = 0.0;
        /// 块高度 / Block height
        double height = 0.0;
        /// 块深度 / Block depth
        double depth = 0.0;
        /// 块重量 / Block weight
        double weight = 0.0;

        /// 从物品视图创建简单块 / Create simple block from item view
        /// 对应 Rust SimpleBlock::from_item_view
        [[nodiscard]] static SimpleBlock from_item_view(
            ItemView view, std::size_t nx_val, std::size_t ny_val, std::size_t nz_val)
        {
            SimpleBlock block;
            block.item_view = view;
            block.nx = nx_val;
            block.ny = ny_val;
            block.nz = nz_val;
            block.item_count = nx_val * ny_val * nz_val;
            block.width = view.width * static_cast<double>(nx_val);
            block.height = view.height * static_cast<double>(ny_val);
            block.depth = view.depth * static_cast<double>(nz_val);
            block.weight = view.weight * static_cast<double>(block.item_count);
            return block;
        }

        /// 块尺寸 / Block size
        [[nodiscard]] MetricSize3 size() const {
            return MetricSize3{width, height, depth};
        }
    };

    // ============================================================================
    // Space 方法实现（需要 SimpleBlock 完整定义）
    // ============================================================================

    inline bool Space::fits(const SimpleBlock& block) const noexcept {
        return block.width <= size.width
            && block.height <= size.height
            && block.depth <= size.depth;
    }

    inline std::vector<Space> Space::place_block(const SimpleBlock& block) const {
        std::vector<Space> sub_spaces;

        // 右侧子空间 / Right sub-space
        double right_width = size.width - block.width;
        if (right_width > 0.0) {
            sub_spaces.push_back({
                {origin.x + block.width, origin.y, origin.z},
                {right_width, size.height, size.depth}
            });
        }

        // 上方子空间 / Above sub-space
        double above_height = size.height - block.height;
        if (above_height > 0.0) {
            sub_spaces.push_back({
                {origin.x, origin.y + block.height, origin.z},
                {block.width, above_height, size.depth}
            });
        }

        // 前方子空间 / Front sub-space
        double front_depth = size.depth - block.depth;
        if (front_depth > 0.0) {
            sub_spaces.push_back({
                {origin.x, origin.y, origin.z + block.depth},
                {block.width, block.height, front_depth}
            });
        }

        return sub_spaces;
    }

    // ============================================================================
    // 块放置 / Block placement
    // 对应 Rust BlockPlacement<V, U>
    // ============================================================================

    /// 块放置 / Block placement
    /// 一个块在容器中的位置信息
    struct BlockPlacement {
        /// 放置位置 / Placement position
        MetricPoint3 position;
        /// 块引用索引 / Block reference index
        std::size_t block_index = 0;
    };

}  // namespace ospf::framework::bpp3d
