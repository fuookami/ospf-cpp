#pragma once

/// 可选数组 / Optional array
/// 固定大小数组，每个元素可能为空。

#include <cstddef>
#include <optional>
#include <vector>

namespace ospf::base {

    /// 可选数组 / Optional array
    template<typename T>
    class OptionalArray {
    public:
        explicit OptionalArray(std::size_t size) : data_(size) {}

        [[nodiscard]] std::optional<T>& operator[](std::size_t i) { return data_[i]; }
        [[nodiscard]] const std::optional<T>& operator[](std::size_t i) const { return data_[i]; }

        [[nodiscard]] std::size_t size() const { return data_.size(); }
        [[nodiscard]] bool empty() const { return data_.empty(); }

        void clear() {
            for (auto& opt : data_) {
                opt.reset();
            }
        }

    private:
        std::vector<std::optional<T>> data_;
    };

}  // namespace ospf::base
