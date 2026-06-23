#pragma once

/// 容器概念 / Container concepts

#include <concepts>
#include <ranges>
#include <type_traits>

namespace ospf::base {

    /// 容器：可迭代且有 size / Container: iterable with size
    template<typename T>
    concept Container = requires(T& t) {
        { std::ranges::begin(t) } -> std::input_or_output_iterator;
        { std::ranges::end(t) } -> std::sentinel_for<decltype(std::ranges::begin(t))>;
        { t.size() } -> std::convertible_to<std::size_t>;
    };

    /// 可索引容器 / Indexable container
    template<typename T>
    concept Indexable = Container<T> && requires(T& t, std::size_t i) {
        { t[i] };
    };

    /// 可追加容器 / Appendable container
    template<typename T>
    concept Appendable = requires(T& t, typename T::value_type v) {
        t.push_back(std::move(v));
    };

}  // namespace ospf::base
