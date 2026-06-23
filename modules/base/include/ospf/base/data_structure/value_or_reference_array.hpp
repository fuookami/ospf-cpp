#pragma once

/// 值或引用数组 / Value-or-reference array
/// 可以存储值或引用的数组，通过 variant 实现。

#include <cstddef>
#include <variant>
#include <vector>

namespace ospf::base {

    /// 值或引用数组 / Value-or-reference array
    template<typename T>
    class ValueOrReferenceArray {
    public:
        void add_value(T value) {
            data_.emplace_back(std::move(value));
        }

        void add_ref(T* ref) {
            data_.emplace_back(ref);
        }

        [[nodiscard]] T& get(std::size_t i) {
            auto& entry = data_[i];
            if (std::holds_alternative<T>(entry)) {
                return std::get<T>(entry);
            }
            return *std::get<T*>(entry);
        }

        [[nodiscard]] const T& get(std::size_t i) const {
            const auto& entry = data_[i];
            if (std::holds_alternative<T>(entry)) {
                return std::get<T>(entry);
            }
            return *std::get<T*>(entry);
        }

        [[nodiscard]] std::size_t size() const { return data_.size(); }

    private:
        std::vector<std::variant<T, T*>> data_;
    };

}  // namespace ospf::base
