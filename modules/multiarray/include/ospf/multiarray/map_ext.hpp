#pragma once

/// 映射扩展 / Map extensions
/// 1:1 对应 Rust map_ext.rs

#include <unordered_map>
#include <vector>
#include <functional>

namespace ospf::multiarray {

    /// 映射扩展工具 / Map extension utilities
    namespace map_ext {

        /// 对映射中每个值应用函数 / Apply function to each value in map
        template<typename K, typename V, typename F>
        [[nodiscard]] auto map_values(const std::unordered_map<K, V>& map, F&& f)
            -> std::unordered_map<K, std::invoke_result_t<F, const V&>>
        {
            using U = std::invoke_result_t<F, const V&>;
            std::unordered_map<K, U> result;
            for (const auto& [k, v] : map) {
                result.emplace(k, f(v));
            }
            return result;
        }

        /// 过滤映射 / Filter map
        template<typename K, typename V, typename Pred>
        [[nodiscard]] auto filter(const std::unordered_map<K, V>& map, Pred&& pred)
            -> std::unordered_map<K, V>
        {
            std::unordered_map<K, V> result;
            for (const auto& [k, v] : map) {
                if (pred(k, v)) {
                    result.emplace(k, v);
                }
            }
            return result;
        }

        /// 合并两个映射 / Merge two maps
        template<typename K, typename V>
        void merge(std::unordered_map<K, V>& target, const std::unordered_map<K, V>& source) {
            for (const auto& [k, v] : source) {
                target[k] = v;
            }
        }

    }  // namespace map_ext

}  // namespace ospf::multiarray
