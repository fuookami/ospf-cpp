#pragma once

/// 多维数组核心概念 / Multi-dimensional array core concepts
/// 存储顺序、访问顺序、维度标记。
/// 1:1 对应 Rust concept.rs — StorageOrderTrait 桥接编译期类型与运行时枚举。

#include <array>
#include <cstddef>
#include <limits>
#include <vector>

namespace ospf::multiarray {

    /// 动态维度标记 / Dynamic dimension marker
    inline constexpr std::size_t DYN_DIMENSION = std::numeric_limits<std::size_t>::max();

    // ============================================================================
    // StorageOrder — 运行时枚举 / Runtime enum
    // ============================================================================

    /// 存储顺序枚举（运行时） / Storage order enum (runtime)
    enum class StorageOrder : std::uint8_t {
        RowMajor,     ///< 行优先（C 风格） / Row-major (C-style)
        ColumnMajor,  ///< 列优先（Fortran 风格） / Column-major (Fortran-style)
    };

    /// 访问顺序枚举（运行时） / Access order enum (runtime)
    enum class AccessOrder : std::uint8_t {
        RowMajor,
        ColumnMajor,
    };

    /// 从存储顺序获取访问顺序 / Get access order from storage order
    [[nodiscard]] constexpr AccessOrder access_order_from_storage(StorageOrder so) noexcept {
        return so == StorageOrder::RowMajor ? AccessOrder::RowMajor : AccessOrder::ColumnMajor;
    }

    // ============================================================================
    // RowMajor / ColumnMajor — 编译期类型 / Compile-time types
    // ============================================================================

    /// 行优先标记类型 / Row-major marker type
    struct RowMajorTag {
        static constexpr StorageOrder runtime_value = StorageOrder::RowMajor;
        static constexpr AccessOrder access_value = AccessOrder::RowMajor;
    };

    /// 列优先标记类型 / Column-major marker type
    struct ColumnMajorTag {
        static constexpr StorageOrder runtime_value = StorageOrder::ColumnMajor;
        static constexpr AccessOrder access_value = AccessOrder::ColumnMajor;
    };

    // ============================================================================
    // StorageOrderTrait — concept 桥接编译期与运行时 / Concept bridging compile-time and runtime
    // ============================================================================

    /// 存储顺序 trait / Storage order trait
    /// 1:1 对应 Rust StorageOrderTrait：要求 runtime_value() 和 offsets() 方法。
    template<typename SO>
    concept StorageOrderTrait = requires {
        { SO::runtime_value } -> std::convertible_to<StorageOrder>;
        { SO::access_value } -> std::convertible_to<AccessOrder>;
    };

    // ============================================================================
    // StorageOrderTraitImpl — 偏特化实现 / Partial specialization implementations
    // ============================================================================

    /// 存储顺序 trait 实现（通用） / Storage order trait implementation (generic)
    template<StorageOrderTrait SO>
    struct StorageOrderOps {
        /// 运行时值 / Runtime value
        static constexpr StorageOrder order = SO::runtime_value;
        static constexpr AccessOrder access = SO::access_value;

        /// 编译期偏移量计算 / Compile-time offset calculation
        template<std::size_t N>
        [[nodiscard]] static constexpr std::array<std::size_t, N> offsets(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::array<std::size_t, N> off{};
            if constexpr (N == 0) return off;
            if constexpr (std::is_same_v<SO, RowMajorTag>) {
                off[N - 1] = 1;
                for (std::size_t i = N - 1; i > 0; --i) {
                    off[i - 1] = off[i] * shape[i];
                }
            } else {
                off[0] = 1;
                for (std::size_t i = 1; i < N; ++i) {
                    off[i] = off[i - 1] * shape[i - 1];
                }
            }
            return off;
        }

        /// 编译期总大小 / Compile-time total size
        template<std::size_t N>
        [[nodiscard]] static constexpr std::size_t total_size(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::size_t s = 1;
            for (std::size_t i = 0; i < N; ++i) s *= shape[i];
            return s;
        }

        /// 动态偏移量计算 / Dynamic offset calculation
        [[nodiscard]] static std::vector<std::size_t> dyn_offsets(
            const std::vector<std::size_t>& shape)
        {
            auto n = shape.size();
            std::vector<std::size_t> off(n, 0);
            if (n == 0) return off;
            if constexpr (std::is_same_v<SO, RowMajorTag>) {
                off[n - 1] = 1;
                for (std::size_t i = n - 1; i > 0; --i) {
                    off[i - 1] = off[i] * shape[i];
                }
            } else {
                off[0] = 1;
                for (std::size_t i = 1; i < n; ++i) {
                    off[i] = off[i - 1] * shape[i - 1];
                }
            }
            return off;
        }
    };

    // ============================================================================
    // 向后兼容：StorageOrderTrait<Order> 偏特化 / Backward compatibility
    // ============================================================================

    /// 存储顺序 trait（向后兼容版，使用非类型模板参数）
    /// Storage order trait (backward compatibility, uses non-type template parameter)
    template<StorageOrder Order>
    struct StorageOrderTrait_compat;

    template<>
    struct StorageOrderTrait_compat<StorageOrder::RowMajor>
        : StorageOrderOps<RowMajorTag> {};

    template<>
    struct StorageOrderTrait_compat<StorageOrder::ColumnMajor>
        : StorageOrderOps<ColumnMajorTag> {};

}  // namespace ospf::multiarray
