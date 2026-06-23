#pragma once

/// 并行处理工具 / Parallel processing utilities
/// 提供并行 for_each、parallel_reduce 等接口。
/// 注意：实际并行执行需要 <execution> 支持；此处提供串行回退实现。

#include <algorithm>
#include <cstddef>
#include <execution>
#include <functional>
#include <numeric>
#include <vector>

namespace ospf::base {

    /// 并行 for_each（若支持） / Parallel for_each (if supported)
    template<typename Iter, typename Func>
    void parallel_for_each(Iter first, Iter last, Func func) {
#if defined(__cpp_lib_execution)
        std::for_each(std::execution::par_unseq, first, last, func);
#else
        std::for_each(first, last, func);
#endif
    }

    /// 并行 reduce（若支持） / Parallel reduce (if supported)
    template<typename Iter, typename T, typename BinaryOp>
    T parallel_reduce(Iter first, Iter last, T init, BinaryOp op) {
#if defined(__cpp_lib_execution)
        return std::reduce(std::execution::par_unseq, first, last, init, op);
#else
        return std::accumulate(first, last, init, op);
#endif
    }

    /// 分块并行处理 / Chunk-parallel processing
    template<typename T, typename Func>
    void chunked_for_each(std::vector<std::vector<T>>& chunks, Func func) {
        for (auto& chunk : chunks) {
            for (auto& item : chunk) {
                func(item);
            }
        }
    }

}  // namespace ospf::base
