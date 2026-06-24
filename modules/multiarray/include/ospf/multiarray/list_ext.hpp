#pragma once

/// 列表扩展 / List extensions
/// 1:1 对应 Rust list_ext.rs

#include <vector>
#include <algorithm>
#include <numeric>

namespace ospf::multiarray {

    /// 列表扩展工具 / List extension utilities
    namespace list_ext {

        /// 去重 / Remove duplicates (preserving order)
        template<typename T>
        [[nodiscard]] std::vector<T> unique(std::vector<T> vec) {
            auto it = std::unique(vec.begin(), vec.end());
            vec.erase(it, vec.end());
            return vec;
        }

        /// 扁平化 / Flatten nested vectors
        template<typename T>
        [[nodiscard]] std::vector<T> flatten(const std::vector<std::vector<T>>& nested) {
            std::vector<T> result;
            for (const auto& inner : nested) {
                result.insert(result.end(), inner.begin(), inner.end());
            }
            return result;
        }

        /// 分组 / Chunk into groups of size n
        template<typename T>
        [[nodiscard]] std::vector<std::vector<T>> chunk(const std::vector<T>& vec, std::size_t n) {
            std::vector<std::vector<T>> result;
            for (std::size_t i = 0; i < vec.size(); i += n) {
                std::size_t end = std::min(i + n, vec.size());
                result.emplace_back(vec.begin() + i, vec.begin() + end);
            }
            return result;
        }

    }  // namespace list_ext

}  // namespace ospf::multiarray
