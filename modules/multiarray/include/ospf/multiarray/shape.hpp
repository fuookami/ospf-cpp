#pragma once

/// 形状定义 / Shape definitions
/// Shape<N> 编译期维度，DynShape 运行期维度。

#include <ospf/multiarray/concept.hpp>
#include <ospf/base/error/code.hpp>
#include <ospf/base/functional/result.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>
#include <vector>

namespace ospf::multiarray {

    /// 编译期形状 / Compile-time shape
    template<std::size_t N, StorageOrder Order = StorageOrder::RowMajor>
    class Shape {
    public:
        static constexpr std::size_t DIMENSION = N;
        static constexpr StorageOrder STORAGE_ORDER = Order;
        using OrderTrait = StorageOrderTrait<Order>;

        /// 从维度大小数组构造 / Construct from dimension sizes
        explicit constexpr Shape(const std::array<std::size_t, N>& sizes)
            : sizes_(sizes), offsets_(OrderTrait::offsets(sizes)),
              total_size_(OrderTrait::total_size(sizes)) {}

        /// 获取维度数 / Get dimension count
        [[nodiscard]] constexpr std::size_t dimension() const noexcept { return N; }

        /// 获取总元素数 / Get total element count
        [[nodiscard]] constexpr std::size_t size() const noexcept { return total_size_; }

        /// 获取指定维度大小 / Get size of specified dimension
        [[nodiscard]] constexpr std::size_t size(std::size_t dim) const { return sizes_[dim]; }

        /// 获取形状数组 / Get shape array
        [[nodiscard]] constexpr const std::array<std::size_t, N>& sizes() const noexcept { return sizes_; }

        /// 获取偏移量数组 / Get offsets array
        [[nodiscard]] constexpr const std::array<std::size_t, N>& offsets() const noexcept { return offsets_; }

        /// 获取存储顺序 / Get storage order
        [[nodiscard]] constexpr StorageOrder storage_order() const noexcept { return Order; }

        /// 将多维索引转为线性索引 / Convert multi-dimensional index to linear index
        template<typename... Idx>
        [[nodiscard]] constexpr std::size_t index_of(Idx... indices) const {
            static_assert(sizeof...(indices) == N);
            std::array<std::size_t, N> idx{static_cast<std::size_t>(indices)...};
            std::size_t linear = 0;
            for (std::size_t i = 0; i < N; ++i) {
                linear += idx[i] * offsets_[i];
            }
            return linear;
        }

        /// 将线性索引转为多维索引 / Convert linear index to multi-dimensional index
        [[nodiscard]] constexpr std::array<std::size_t, N> vector_of(std::size_t linear) const {
            std::array<std::size_t, N> idx{};
            std::size_t remaining = linear;
            for (std::size_t i = 0; i < N; ++i) {
                idx[i] = remaining / offsets_[i];
                remaining %= offsets_[i];
            }
            return idx;
        }

        /// 检查索引是否在范围内 / Check if index is in range
        template<typename... Idx>
        [[nodiscard]] constexpr bool in_range(Idx... indices) const {
            static_assert(sizeof...(indices) == N);
            std::array<std::size_t, N> idx{static_cast<std::size_t>(indices)...};
            for (std::size_t i = 0; i < N; ++i) {
                if (idx[i] >= sizes_[i]) return false;
            }
            return true;
        }

        [[nodiscard]] constexpr bool operator==(const Shape& other) const noexcept {
            return sizes_ == other.sizes_;
        }

    private:
        std::array<std::size_t, N> sizes_;
        std::array<std::size_t, N> offsets_;
        std::size_t total_size_;
    };

    /// 动态形状 / Dynamic shape (runtime dimension)
    template<StorageOrder Order = StorageOrder::RowMajor>
    class DynShape {
    public:
        using OrderTrait = StorageOrderTrait<Order>;

        /// 从维度大小向量构造 / Construct from dimension sizes vector
        explicit DynShape(std::vector<std::size_t> sizes)
            : sizes_(std::move(sizes)),
              offsets_(OrderTrait::dyn_offsets(sizes_)),
              total_size_(compute_total_size(sizes_)) {}

        /// 获取维度数 / Get dimension count
        [[nodiscard]] std::size_t dimension() const noexcept { return sizes_.size(); }

        /// 获取总元素数 / Get total element count
        [[nodiscard]] std::size_t size() const noexcept { return total_size_; }

        /// 获取指定维度大小 / Get size of specified dimension
        [[nodiscard]] std::size_t size(std::size_t dim) const { return sizes_[dim]; }

        /// 获取形状向量 / Get shape vector
        [[nodiscard]] const std::vector<std::size_t>& sizes() const noexcept { return sizes_; }

        /// 获取偏移量向量 / Get offsets vector
        [[nodiscard]] const std::vector<std::size_t>& offsets() const noexcept { return offsets_; }

        /// 获取存储顺序 / Get storage order
        [[nodiscard]] StorageOrder storage_order() const noexcept { return Order; }

        /// 将多维索引转为线性索引 / Convert multi-dimensional index to linear index
        [[nodiscard]] std::size_t index_of(const std::vector<std::size_t>& idx) const {
            std::size_t linear = 0;
            for (std::size_t i = 0; i < sizes_.size(); ++i) {
                linear += idx[i] * offsets_[i];
            }
            return linear;
        }

        /// 将线性索引转为多维索引 / Convert linear index to multi-dimensional index
        [[nodiscard]] std::vector<std::size_t> vector_of(std::size_t linear) const {
            std::vector<std::size_t> idx(sizes_.size(), 0);
            std::size_t remaining = linear;
            for (std::size_t i = 0; i < sizes_.size(); ++i) {
                idx[i] = remaining / offsets_[i];
                remaining %= offsets_[i];
            }
            return idx;
        }

        [[nodiscard]] bool operator==(const DynShape& other) const noexcept {
            return sizes_ == other.sizes_;
        }

    private:
        static std::size_t compute_total_size(const std::vector<std::size_t>& sizes) {
            if (sizes.empty()) return 0;
            std::size_t s = 1;
            for (auto d : sizes) s *= d;
            return s;
        }

        std::vector<std::size_t> sizes_;
        std::vector<std::size_t> offsets_;
        std::size_t total_size_;
    };

}  // namespace ospf::multiarray
