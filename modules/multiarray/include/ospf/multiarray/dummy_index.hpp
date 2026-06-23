#pragma once

/// 虚拟索引 / Dummy index
/// 用于多维数组的索引操作，支持 ALL（全选）和范围选择。

#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

namespace ospf::multiarray {

    /// 虚拟索引：可以是具体索引、ALL（全选）、或范围 / Dummy index: concrete index, ALL, or range
    class DummyIndex {
    public:
        /// 具体索引 / Concrete index
        [[nodiscard]] static DummyIndex index(std::size_t i) { return DummyIndex(i); }

        /// 全选 / Select all
        [[nodiscard]] static DummyIndex all() { return DummyIndex(ALL_TAG{}); }

        /// 范围 / Range [start, end)
        [[nodiscard]] static DummyIndex range(std::size_t start, std::size_t end) {
            return DummyIndex(Range{start, end});
        }

        [[nodiscard]] bool is_index() const { return std::holds_alternative<std::size_t>(data_); }
        [[nodiscard]] bool is_all()    const { return std::holds_alternative<ALL_TAG>(data_); }
        [[nodiscard]] bool is_range()  const { return std::holds_alternative<Range>(data_); }

        [[nodiscard]] std::size_t get_index() const { return std::get<std::size_t>(data_); }
        [[nodiscard]] std::size_t range_start() const { return std::get<Range>(data_).start; }
        [[nodiscard]] std::size_t range_end()   const { return std::get<Range>(data_).end; }

    private:
        struct ALL_TAG {};
        struct Range { std::size_t start; std::size_t end; };

        explicit DummyIndex(std::size_t i) : data_(i) {}
        explicit DummyIndex(ALL_TAG) : data_(ALL_TAG{}) {}
        explicit DummyIndex(Range r) : data_(r) {}

        std::variant<std::size_t, ALL_TAG, Range> data_;
    };

    /// 索引向量类型 / Index vector type
    using DummyIndexVector = std::vector<DummyIndex>;

}  // namespace ospf::multiarray
