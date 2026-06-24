#pragma once

/// 块数组 / Block multi-array
/// 1:1 对应 Rust block_multi_array.rs
/// 将大数组分块存储，支持并行处理。

#include <ospf/multiarray/multi_array.hpp>
#include <ospf/base/chunked_collection.hpp>
#include <cstddef>

namespace ospf::multiarray {

    /// 块多维数组 / Block multi-array
    /// 使用 ChunkedVec 存储数据，适合大型数组。
    template<typename T, std::size_t N, StorageOrder Order = StorageOrder::RowMajor>
    class BlockMultiArray {
    public:
        using ShapeType = Shape<N, Order>;

        BlockMultiArray(const ShapeType& shape, std::size_t chunk_size = ospf::base::DEFAULT_CHUNK_SIZE)
            : shape_(shape), data_(chunk_size) {
            data_.resize(shape.size(), T{});
        }

        template<typename... Idx>
        [[nodiscard]] T& at(Idx... indices) {
            return data_[shape_.index_of(indices...)];
        }

        template<typename... Idx>
        [[nodiscard]] const T& at(Idx... indices) const {
            auto* ptr = const_cast<BlockMultiArray*>(this)->data_.get(shape_.index_of(indices...));
            return *ptr;
        }

        [[nodiscard]] const ShapeType& shape() const noexcept { return shape_; }
        [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }

        void fill(const T& value) {
            for (std::size_t i = 0; i < data_.size(); ++i) {
                data_[i] = value;
            }
        }

    private:
        ShapeType shape_;
        ospf::base::ChunkedVec<T> data_;
    };

}  // namespace ospf::multiarray
