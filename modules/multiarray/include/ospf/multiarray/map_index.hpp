#pragma once

/// 映射索引 / Map index
/// 用于多维数组的高级索引操作。

#include <cstddef>
#include <optional>
#include <vector>

namespace ospf::multiarray {

    /// 映射索引：将一个维度的索引映射到另一个维度 / Map index
    class MapIndex {
    public:
        /// 恒等映射 / Identity map
        [[nodiscard]] static MapIndex identity(std::size_t size) {
            return MapIndex(size, std::nullopt);
        }

        /// 自定义映射 / Custom map
        [[nodiscard]] static MapIndex custom(std::size_t size, std::vector<std::size_t> mapping) {
            return MapIndex(size, std::move(mapping));
        }

        [[nodiscard]] std::size_t size() const { return size_; }

        [[nodiscard]] std::size_t map(std::size_t i) const {
            if (mapping_.has_value()) return (*mapping_)[i];
            return i;
        }

        [[nodiscard]] bool is_identity() const { return !mapping_.has_value(); }

    private:
        MapIndex(std::size_t size, std::optional<std::vector<std::size_t>> mapping)
            : size_(size), mapping_(std::move(mapping)) {}

        std::size_t size_;
        std::optional<std::vector<std::size_t>> mapping_;
    };

}  // namespace ospf::multiarray
