#pragma once

/// 索引值类型 / Index value type
/// 1:1 对应 Rust index_value.rs

#include <optional>
#include <stdexcept>
#include <string>

namespace ospf::multiarray {

    /// 索引值：支持正索引和负索引（从末尾计数）/ Index value: supports positive and negative indices
    class IndexValue {
    public:
        constexpr explicit IndexValue(std::ptrdiff_t value) : value_(value) {}

        [[nodiscard]] constexpr std::ptrdiff_t value() const noexcept { return value_; }

        /// 解析为实际索引 / Resolve to actual index given dimension size
        [[nodiscard]] constexpr std::size_t resolve(std::size_t dim_size) const {
            if (value_ >= 0) {
                return static_cast<std::size_t>(value_);
            }
            return static_cast<std::size_t>(static_cast<std::ptrdiff_t>(dim_size) + value_);
        }

        [[nodiscard]] constexpr bool operator==(const IndexValue& other) const noexcept = default;
        [[nodiscard]] constexpr auto operator<=>(const IndexValue& other) const noexcept = default;

    private:
        std::ptrdiff_t value_;
    };

    /// 尝试转换为 IndexValue / Try converting to IndexValue
    template<typename T>
    [[nodiscard]] constexpr std::optional<IndexValue> try_into_index_value(T value) {
        return IndexValue(static_cast<std::ptrdiff_t>(value));
    }

}  // namespace ospf::multiarray
