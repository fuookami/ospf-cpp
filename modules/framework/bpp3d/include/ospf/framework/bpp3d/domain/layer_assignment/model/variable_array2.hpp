#pragma once
/// BPP3D domain/layer_assignment/model/variable_array2 /// 1:1 对应 Rust bpp3d/domain/layer_assignment/model/variable_array2.rs
/// VariableArray2 — 将双键域键映射到模型变量索引
/// Maps dual-key domain keys to model variable indices.

#include <cstddef>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>

namespace ospf::framework::bpp3d {

    /// 二维索引变量集合 / 2D indexed variable array
    /// 对应 Rust VariableArray2<K1, K2, VT>
    template<typename K1, typename K2>
    class VariableArray2 {
    public:
        using Key = std::pair<K1, K2>;

        explicit VariableArray2(std::string prefix)
            : prefix_(std::move(prefix)) {}

        [[nodiscard]] const std::string& prefix() const noexcept { return prefix_; }
        [[nodiscard]] std::size_t size() const noexcept { return indices_.size(); }
        [[nodiscard]] bool empty() const noexcept { return indices_.empty(); }

        [[nodiscard]] std::optional<std::size_t> index(const K1& k1, const K2& k2) const {
            auto it = indices_.find({k1, k2});
            if (it != indices_.end()) return it->second;
            return std::nullopt;
        }

        void register_key(const K1& k1, const K2& k2, std::size_t idx) {
            indices_[{k1, k2}] = idx;
        }

        [[nodiscard]] const std::unordered_map<Key, std::size_t, PairHash>& indices() const noexcept {
            return indices_;
        }

    private:
        std::string prefix_;
        struct PairHash {
            std::size_t operator()(const Key& k) const noexcept {
                auto h1 = std::hash<K1>{}(k.first);
                auto h2 = std::hash<K2>{}(k.second);
                return h1 ^ (h2 * 2654435761u);
            }
        };
        std::unordered_map<Key, std::size_t, PairHash> indices_;
    };

}  // namespace ospf::framework::bpp3d
