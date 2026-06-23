#pragma once

/// 集合索引抽象 / Collection index abstractions
/// 提供 Indices trait（concept），用于获取有效索引范围。

#include <cstddef>
#include <ranges>
#include <vector>

namespace ospf::base {

    /// 提供索引范围的类型 / Types that provide index ranges
    template<typename T>
    concept HasIndices = requires(const T& t) {
        { t.indices() } -> std::convertible_to<std::size_t>;
    };

    /// 获取有效索引范围 / Get valid index range
    [[nodiscard]] constexpr std::size_t indices(std::size_t n) noexcept {
        return n;
    }

    template<typename T>
    [[nodiscard]] std::size_t indices(const std::vector<T>& v) noexcept {
        return v.size();
    }

}  // namespace ospf::base
