#pragma once

/// 基础概念 / Base concepts

#include <concepts>
#include <type_traits>

namespace ospf::base {

    /// 可比较 / Comparable
    template<typename T>
    concept Comparable = requires(const T& a, const T& b) {
        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;
    };

    /// 可排序 / Ordered
    template<typename T>
    concept Ordered = Comparable<T> && requires(const T& a, const T& b) {
        { a < b } -> std::convertible_to<bool>;
        { a <= b } -> std::convertible_to<bool>;
        { a > b } -> std::convertible_to<bool>;
        { a >= b } -> std::convertible_to<bool>;
    };

    /// 可哈希 / Hashable
    template<typename T>
    concept Hashable = requires(const T& a) {
        { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    };

    /// 可克隆 / Cloneable (trivially or copy-constructible)
    template<typename T>
    concept Cloneable = std::copy_constructible<T>;

    /// 可移动 / Movable
    template<typename T>
    concept Movable = std::move_constructible<T>;

}  // namespace ospf::base
