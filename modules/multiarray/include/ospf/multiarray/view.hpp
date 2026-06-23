#pragma once

/// MultiArray 视图 / Multi-array view
/// 非拥有的数组视图，支持切片和子视图。

#include <ospf/multiarray/shape.hpp>
#include <cstddef>
#include <vector>

namespace ospf::multiarray {

    /// 多维数组视图 / Multi-array view (non-owning)
    template<typename T, std::size_t N, StorageOrder Order = StorageOrder::RowMajor>
    class MultiArrayView {
    public:
        using ShapeType = Shape<N, Order>;

        MultiArrayView(T* data, const ShapeType& shape)
            : data_(data), shape_(shape) {}

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

        [[nodiscard]] const ShapeType& shape() const noexcept { return shape_; }
        [[nodiscard]] std::size_t size() const noexcept { return shape_.size(); }
        [[nodiscard]] T* data() noexcept { return data_; }
        [[nodiscard]] const T* data() const noexcept { return data_; }

    private:
        T* data_;
        ShapeType shape_;
    };

    /// 动态维度视图 / Dynamic dimension view
    template<typename T, StorageOrder Order = StorageOrder::RowMajor>
    class DynMultiArrayView {
    public:
        using ShapeType = DynShape<Order>;

        DynMultiArrayView(T* data, const ShapeType& shape)
            : data_(data), shape_(shape) {}

        [[nodiscard]] T& at(const std::vector<std::size_t>& idx) {
            return data_[shape_.index_of(idx)];
        }

        [[nodiscard]] const T& at(const std::vector<std::size_t>& idx) const {
            return data_[shape_.index_of(idx)];
        }

        [[nodiscard]] T& operator[](std::size_t linear) { return data_[linear]; }
        [[nodiscard]] const T& operator[](std::size_t linear) const { return data_[linear]; }

        [[nodiscard]] const ShapeType& shape() const noexcept { return shape_; }
        [[nodiscard]] std::size_t size() const noexcept { return shape_.size(); }
        [[nodiscard]] T* data() noexcept { return data_; }
        [[nodiscard]] const T* data() const noexcept { return data_; }

    private:
        T* data_;
        ShapeType shape_;
    };

}  // namespace ospf::multiarray
