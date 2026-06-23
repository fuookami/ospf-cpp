#pragma once

/// 标签映射 / Tagged map
/// 通过标签键关联不同类型值的映射。

#include <any>
#include <string>
#include <unordered_map>

namespace ospf::base {

    /// 标签映射：按字符串标签存储任意类型值 / Tagged map: stores any-typed values by string tag
    class TaggedMap {
    public:
        template<typename T>
        void set(const std::string& tag, T value) {
            data_[tag] = std::move(value);
        }

        template<typename T>
        [[nodiscard]] T* get(const std::string& tag) {
            auto it = data_.find(tag);
            if (it == data_.end()) return nullptr;
            return std::any_cast<T>(&it->second);
        }

        template<typename T>
        [[nodiscard]] const T* get(const std::string& tag) const {
            auto it = data_.find(tag);
            if (it == data_.end()) return nullptr;
            return std::any_cast<T>(&it->second);
        }

        [[nodiscard]] bool contains(const std::string& tag) const {
            return data_.count(tag) > 0;
        }

        [[nodiscard]] std::size_t size() const { return data_.size(); }
        [[nodiscard]] bool empty() const { return data_.empty(); }

        void clear() { data_.clear(); }

    private:
        std::unordered_map<std::string, std::any> data_;
    };

}  // namespace ospf::base
