#pragma once

/// 迭代器扩展 / Iterator extensions
/// 提供 none() 等迭代器适配器。

#include <algorithm>
#include <iterator>

namespace ospf::base {

    /// 判断迭代器中没有元素满足条件 / Check if no elements satisfy predicate
    template<typename Iter, typename Pred>
    [[nodiscard]] bool none(Iter first, Iter last, Pred pred) {
        return std::none_of(first, last, pred);
    }

    /// 判断迭代器中没有元素满足条件（范围版本）/ Range version
    template<typename Range, typename Pred>
    [[nodiscard]] bool none(Range&& range, Pred pred) {
        return std::none_of(std::begin(range), std::end(range), pred);
    }

}  // namespace ospf::base
