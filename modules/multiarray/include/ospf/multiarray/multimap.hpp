#pragma once

/// 多重映射 / Multi-map
/// 1:1 对应 Rust multimap.rs

#include <unordered_map>
#include <vector>
#include <algorithm>

namespace ospf::multiarray {

    /// 多重映射：一个键对应多个值 / Multi-map: one key maps to multiple values
    template<typename K, typename V>
    class MultiMap {
    public:
        using iterator = typename std::unordered_map<K, std::vector<V>>::iterator;
        using const_iterator = typename std::unordered_map<K, std::vector<V>>::const_iterator;

        MultiMap() = default;

        /// 插入键值对 / Insert key-value pair
        void insert(const K& key, V value) {
            data_[key].push_back(std::move(value));
        }

        /// 获取键对应的所有值 / Get all values for key
        [[nodiscard]] const std::vector<V>* get(const K& key) const {
            auto it = data_.find(key);
            return it != data_.end() ? &it->second : nullptr;
        }

        /// 获取可变引用 / Get mutable reference
        [[nodiscard]] std::vector<V>* get_mut(const K& key) {
            auto it = data_.find(key);
            return it != data_.end() ? &it->second : nullptr;
        }

        /// 是否包含键 / Contains key
        [[nodiscard]] bool contains(const K& key) const {
            return data_.count(key) > 0;
        }

        /// 键数量 / Key count
        [[nodiscard]] std::size_t size() const { return data_.size(); }

        /// 是否为空 / Is empty
        [[nodiscard]] bool empty() const { return data_.empty(); }

        [[nodiscard]] iterator begin() { return data_.begin(); }
        [[nodiscard]] iterator end() { return data_.end(); }
        [[nodiscard]] const_iterator begin() const { return data_.begin(); }
        [[nodiscard]] const_iterator end() const { return data_.end(); }

    private:
        std::unordered_map<K, std::vector<V>> data_;
    };

}  // namespace ospf::multiarray
