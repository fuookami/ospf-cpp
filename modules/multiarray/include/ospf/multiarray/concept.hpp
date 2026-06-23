#pragma once

/// 多维数组核心概念 / Multi-dimensional array core concepts
/// 存储顺序、访问顺序、维度标记。

#include <array>
#include <cstddef>
#include <vector>

namespace ospf::multiarray {

    /// 动态维度标记 / Dynamic dimension marker
    inline constexpr std::size_t DYN_DIMENSION = std::numeric_limits<std::size_t>::max();

    /// 存储顺序枚举 / Storage order enum
    enum class StorageOrder : std::uint8_t {
        RowMajor,     ///< 行优先（C 风格） / Row-major (C-style)
        ColumnMajor,  ///< 列优先（Fortran 风格） / Column-major (Fortran-style)
    };

    /// 访问顺序枚举 / Access order enum
    enum class AccessOrder : std::uint8_t {
        RowMajor,
        ColumnMajor,
    };

    /// 从存储顺序获取访问顺序 / Get access order from storage order
    [[nodiscard]] constexpr AccessOrder access_order_from_storage(StorageOrder so) noexcept {
        return so == StorageOrder::RowMajor ? AccessOrder::RowMajor : AccessOrder::ColumnMajor;
    }

    /// 存储顺序 trait / Storage order trait
    template<StorageOrder Order>
    struct StorageOrderTrait;

    /// 行优先特化 / Row-major specialization
    template<>
    struct StorageOrderTrait<StorageOrder::RowMajor> {
        static constexpr StorageOrder order = StorageOrder::RowMajor;
        static constexpr AccessOrder access = AccessOrder::RowMajor;

        /// 计算偏移量（从后向前） / Calculate offsets (back to front)
        template<std::size_t N>
        [[nodiscard]] static constexpr std::array<std::size_t, N> offsets(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::array<std::size_t, N> off{};
            if constexpr (N == 0) return off;
            off[N - 1] = 1;
            for (std::size_t i = N - 1; i > 0; --i) {
                off[i - 1] = off[i] * shape[i];
            }
            return off;
        }

        /// 计算总大小 / Calculate total size
        template<std::size_t N>
        [[nodiscard]] static constexpr std::size_t total_size(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::size_t s = 1;
            for (std::size_t i = 0; i < N; ++i) s *= shape[i];
            return s;
        }

        /// 动态版本偏移量 / Dynamic version offsets
        [[nodiscard]] static std::vector<std::size_t> dyn_offsets(
            const std::vector<std::size_t>& shape)
        {
            auto n = shape.size();
            std::vector<std::size_t> off(n, 0);
            if (n == 0) return off;
            off[n - 1] = 1;
            for (std::size_t i = n - 1; i > 0; --i) {
                off[i - 1] = off[i] * shape[i];
            }
            return off;
        }
    };

    /// 列优先特化 / Column-major specialization
    template<>
    struct StorageOrderTrait<StorageOrder::ColumnMajor> {
        static constexpr StorageOrder order = StorageOrder::ColumnMajor;
        static constexpr AccessOrder access = AccessOrder::ColumnMajor;

        template<std::size_t N>
        [[nodiscard]] static constexpr std::array<std::size_t, N> offsets(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::array<std::size_t, N> off{};
            if constexpr (N == 0) return off;
            off[0] = 1;
            for (std::size_t i = 1; i < N; ++i) {
                off[i] = off[i - 1] * shape[i - 1];
            }
            return off;
        }

        template<std::size_t N>
        [[nodiscard]] static constexpr std::size_t total_size(
            const std::array<std::size_t, N>& shape) noexcept
        {
            std::size_t s = 1;
            for (std::size_t i = 0; i < N; ++i) s *= shape[i];
            return s;
        }

        [[nodiscard]] static std::vector<std::size_t> dyn_offsets(
            const std::vector<std::size_t>& shape)
        {
            auto n = shape.size();
            std::vector<std::size_t> off(n, 0);
            if (n == 0) return off;
            off[0] = 1;
            for (std::size_t i = 1; i < n; ++i) {
                off[i] = off[i - 1] * shape[i - 1];
            }
            return off;
        }
    };

}  // namespace ospf::multiarray
