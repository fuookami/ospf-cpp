#pragma once

/// 引用数组 / Reference array
/// 存储引用包装器的数组。

#include <cstddef>
#include <functional>
#include <vector>

namespace ospf::base {

    /// 引用数组 / Reference array
    template<typename T>
    class ReferenceArray {
    public:
        void add(std::reference_wrapper<T> ref) {
            data_.push_back(ref);
        }

        [[nodiscard]] T& get(std::size_t i) { return data_[i].get(); }
        [[nodiscard]] const T& get(std::size_t i) const { return data_[i].get(); }

        [[nodiscard]] std::size_t size() const { return data_.size(); }
        [[nodiscard]] bool empty() const { return data_.empty(); }

    private:
        std::vector<std::reference_wrapper<T>> data_;
    };

}  // namespace ospf::base
