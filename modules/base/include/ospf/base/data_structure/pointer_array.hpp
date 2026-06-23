#pragma once

/// 指针数组 / Pointer array
/// 存储 unique_ptr 的数组。

#include <cstddef>
#include <memory>
#include <vector>

namespace ospf::base {

    /// 指针数组 / Pointer array
    template<typename T>
    class PointerArray {
    public:
        explicit PointerArray(std::size_t size) : data_(size) {}

        [[nodiscard]] T* get(std::size_t i) { return data_[i].get(); }
        [[nodiscard]] const T* get(std::size_t i) const { return data_[i].get(); }

        void set(std::size_t i, std::unique_ptr<T> ptr) {
            data_[i] = std::move(ptr);
        }

        template<typename... Args>
        T& emplace(std::size_t i, Args&&... args) {
            data_[i] = std::make_unique<T>(std::forward<Args>(args)...);
            return *data_[i];
        }

        [[nodiscard]] std::size_t size() const { return data_.size(); }

    private:
        std::vector<std::unique_ptr<T>> data_;
    };

}  // namespace ospf::base
