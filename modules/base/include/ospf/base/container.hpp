#pragma once

/// 容器类型抽象 / Container type abstractions
/// 提供容器标记类型和概念。

#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>

namespace ospf::base {

    // -- 容器概念 / Container concepts -----------------------------------------

    /// 序列容器 / Sequence container
    template<typename T>
    concept SequenceContainer = requires {
        typename T::value_type;
        typename T::iterator;
        typename T::size_type;
    };

    /// 关联容器 / Associative container
    template<typename T>
    concept AssociativeContainer = SequenceContainer<T> && requires {
        typename T::key_type;
    };

    // -- 容器标记类型 / Container marker types ---------------------------------

    /// Vec 容器标记 / Vec container marker
    struct VecContainer {
        template<typename T>
        using Type = std::vector<T>;
    };

    /// VecDeque 容器标记 / VecDeque container marker
    struct VecDequeContainer {
        template<typename T>
        using Type = std::deque<T>;
    };

    /// List 容器标记 / List container marker
    struct ListContainer {
        template<typename T>
        using Type = std::list<T>;
    };

    /// HashSet 容器标记 / HashSet container marker
    struct HashSetContainer {
        template<typename T>
        using Type = std::unordered_set<T>;
    };

    /// TreeSet 容器标记 / TreeSet container marker
    struct TreeSetContainer {
        template<typename T>
        using Type = std::set<T>;
    };

    /// HashMap 容器标记 / HashMap container marker
    struct HashMapContainer {
        template<typename K, typename V>
        using Type = std::unordered_map<K, V>;
    };

    /// TreeMap 容器标记 / TreeMap container marker
    struct TreeMapContainer {
        template<typename K, typename V>
        using Type = std::map<K, V>;
    };

}  // namespace ospf::base
