#pragma once

/// 快速求和 / Fast sum utilities
/// 1:1 对应 Rust fast_sum.rs

#include <vector>
#include <cstddef>
#include <numeric>

namespace ospf::multiarray {

    /// 快速求和：对多维数组按指定轴求和 / Fast sum: sum along specified axis
    namespace fast_sum {

        /// 沿最后一维求和 / Sum along the last dimension
        template<typename T>
        [[nodiscard]] std::vector<T> sum_last_dim(
            const std::vector<T>& data,
            std::size_t outer_size,
            std::size_t inner_size
        ) {
            std::vector<T> result(outer_size, T{});
            for (std::size_t i = 0; i < outer_size; ++i) {
                T sum{};
                for (std::size_t j = 0; j < inner_size; ++j) {
                    sum += data[i * inner_size + j];
                }
                result[i] = sum;
            }
            return result;
        }

        /// 沿第一维求和 / Sum along the first dimension
        template<typename T>
        [[nodiscard]] std::vector<T> sum_first_dim(
            const std::vector<T>& data,
            std::size_t outer_size,
            std::size_t inner_size
        ) {
            std::vector<T> result(inner_size, T{});
            for (std::size_t i = 0; i < outer_size; ++i) {
                for (std::size_t j = 0; j < inner_size; ++j) {
                    result[j] += data[i * inner_size + j];
                }
            }
            return result;
        }

    }  // namespace fast_sum

}  // namespace ospf::multiarray
