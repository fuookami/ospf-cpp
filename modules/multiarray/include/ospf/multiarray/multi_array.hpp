#pragma once

/// MultiArray — 多维数组容器 / Multi-dimensional array container
/// 1:1 对应 Rust multi_array.rs
/// 支持编译期和运行期维度，行优先/列优先存储。
/// SO 是类型参数（RowMajorTag / ColumnMajorTag），支持静态和动态存储顺序。

#include <ospf/multiarray/shape.hpp>
#include <cstddef>
#include <vector>
#include <functional>
#include <algorithm>

namespace ospf::multiarray {

    // ============================================================================
    // MultiArray<T, N, SO> — 编译期维度多维数组 / Compile-time dimension multi-array
    // ============================================================================

    /// 编译期维度多维数组 / Compile-time dimension multi-array
    template<typename T, std::size_t N, StorageOrderTrait SO = RowMajorTag>
    class MultiArray {
    public:
        using ShapeType = Shape<N, SO>;
        using value_type = T;

        /// 从形状构造（元素默认初始化） / Construct from shape
        explicit MultiArray(const ShapeType& shape)
            : shape_(shape), data_(shape.size()) {}

        /// 从形状和初始值构造 / Construct from shape and initial value
        MultiArray(const ShapeType& shape, const T& init)
            : shape_(shape), data_(shape.size(), init) {}

        // -- 元素访问 / Element access ------------------------------------------

        template<typename... Idx>
        [[nodiscard]] T& at(Idx... indices) {
            return data_[shape_.index_of(indices...)];
        }

        template<typename... Idx>
        [[nodiscard]] const T& at(Idx... indices) const {
            return data_[shape_.index_of(indices...)];
        }

        [[nodiscard]] T& operator[](std::size_t linear) { return data_[linear]; }
        [[nodiscard]] const T& operator[](std::size_t linear) const { return data_[linear]; }

        // -- 形状信息 / Shape info ----------------------------------------------

        [[nodiscard]] const ShapeType& shape() const noexcept { return shape_; }
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        [[nodiscard]] std::size_t dimension() const noexcept { return N; }

        /// 获取存储顺序（运行时） / Get storage order (runtime)
        [[nodiscard]] constexpr StorageOrder storage_order() const noexcept {
            return SO::runtime_value;
        }

        // -- 数据访问 / Data access ---------------------------------------------

        [[nodiscard]] T* data() noexcept { return data_.data(); }
        [[nodiscard]] const T* data() const noexcept { return data_.data(); }

        [[nodiscard]] auto begin() { return data_.begin(); }
        [[nodiscard]] auto end()   { return data_.end(); }
        [[nodiscard]] auto begin() const { return data_.begin(); }
        [[nodiscard]] auto end()   const { return data_.end(); }

        // -- 填充 / Fill --------------------------------------------------------

        void fill(const T& value) {
            std::fill(data_.begin(), data_.end(), value);
        }

        // -- 迭代多维索引 / Iterate multi-dimensional indices --------------------

        /// 按多维索引顺序遍历并调用回调 / Visit in multi-dimensional index order
        template<typename Func>
        void visit(Func&& func) const {
            std::array<std::size_t, N> idx{};
            for (std::size_t linear = 0; linear < data_.size(); ++linear) {
                func(idx, data_[linear]);
                for (std::size_t i = N; i > 0; --i) {
                    if (++idx[i - 1] < shape_.size(i - 1)) break;
                    idx[i - 1] = 0;
                }
            }
        }

    private:
        ShapeType shape_;
        std::vector<T> data_;
    };

    // ============================================================================
    // DynMultiArray<T, SO> — 动态维度多维数组 / Dynamic dimension multi-array
    // ============================================================================

    /// 动态维度多维数组 / Dynamic dimension multi-array
    template<typename T, StorageOrderTrait SO = RowMajorTag>
    class DynMultiArray {
    public:
        using ShapeType = DynShape<SO>;
        using value_type = T;

        explicit DynMultiArray(const ShapeType& shape)
            : shape_(shape), data_(shape.size()) {}

        DynMultiArray(const ShapeType& shape, const T& init)
            : shape_(shape), data_(shape.size(), init) {}

        [[nodiscard]] T& at(const std::vector<std::size_t>& idx) {
            return data_[shape_.index_of(idx)];
        }

        [[nodiscard]] const T& at(const std::vector<std::size_t>& idx) const {
            return data_[shape_.index_of(idx)];
        }

        [[nodiscard]] T& operator[](std::size_t linear) { return data_[linear]; }
        [[nodiscard]] const T& operator[](std::size_t linear) const { return data_[linear]; }

        [[nodiscard]] const ShapeType& shape() const noexcept { return shape_; }
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
        [[nodiscard]] std::size_t dimension() const noexcept { return shape_.dimension(); }

        /// 获取存储顺序（运行时） / Get storage order (runtime)
        [[nodiscard]] constexpr StorageOrder storage_order() const noexcept {
            return SO::runtime_value;
        }

        [[nodiscard]] T* data() noexcept { return data_.data(); }
        [[nodiscard]] const T* data() const noexcept { return data_.data(); }

        void fill(const T& value) {
            std::fill(data_.begin(), data_.end(), value);
        }

    private:
        ShapeType shape_;
        std::vector<T> data_;
    };

}  // namespace ospf::multiarray
