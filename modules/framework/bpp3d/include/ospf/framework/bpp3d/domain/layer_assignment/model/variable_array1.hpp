#pragma once
/// BPP3D domain/layer_assignment/model/variable_array1 /// 1:1 对应 Rust bpp3d/domain/layer_assignment/model/variable_array1.rs
/// VariableArray1 — 将领域键映射到模型变量索引
/// Maps domain keys to model variable indices, supporting registration and solution extraction.

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>

namespace ospf::framework::bpp3d {

    // ============================================================================
    // 一维索引变量集合 / 1D indexed variable array
    // 对应 Rust VariableArray1<K, VT>
    // ============================================================================

    /// 一维索引变量集合 / 1D indexed variable array
    /// 将领域键映射到模型变量索引，支持注册和结果提取。
    /// Maps domain keys to model variable indices, supporting registration and
    /// solution extraction.
    template<typename K>
    class VariableArray1 {
    public:
        /// 创建一维变量集合 / Create a 1D variable array
        /// 对应 Rust VariableArray1::new(prefix)
        explicit VariableArray1(std::string prefix)
            : prefix_(std::move(prefix)) {}

        /// 获取变量名前缀 / Get variable name prefix
        [[nodiscard]] const std::string& prefix() const noexcept { return prefix_; }

        /// 获取变量数量 / Get number of variables
        [[nodiscard]] std::size_t size() const noexcept { return indices_.size(); }

        /// 是否为空 / Whether empty
        [[nodiscard]] bool empty() const noexcept { return indices_.empty(); }

        /// 获取键对应的模型索引 / Get model index for key
        /// 对应 Rust VariableArray1::index
        [[nodiscard]] std::optional<std::size_t> index(const K& key) const {
            auto it = indices_.find(key);
            if (it != indices_.end()) return it->second;
            return std::nullopt;
        }

        /// 注册键并分配索引 / Register key and assign index
        /// 对应 Rust VariableArray1::register_key
        void register_key(const K& key, std::size_t idx) {
            indices_[key] = idx;
        }

        /// 注册连续键（批量） / Register sequential keys (batch)
        /// 对应 Rust VariableArray1::register_continuous
        void register_continuous(const K* keys, std::size_t count, std::size_t start_idx) {
            for (std::size_t i = 0; i < count; ++i) {
                indices_[keys[i]] = start_idx + i;
            }
        }

        /// 生成变量名 / Generate variable name
        /// 对应 Rust VariableArray1::variable_name
        [[nodiscard]] std::string variable_name(const K& key) const {
            return prefix_ + "_" + key_to_string(key);
        }

        /// 获取全部键索引映射 / Get all key-index mappings
        [[nodiscard]] const std::unordered_map<K, std::size_t>& indices() const noexcept {
            return indices_;
        }

    private:
        std::string prefix_;
        std::unordered_map<K, std::size_t> indices_;

        static std::string key_to_string(const K& key) {
            if constexpr (std::is_convertible_v<K, std::string>) {
                return static_cast<std::string>(key);
            } else {
                return std::to_string(key);
            }
        }
    };

}  // namespace ospf::framework::bpp3d
