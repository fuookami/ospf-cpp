#pragma once

/// 带标签概念 / With-tag concept

#include <string>

namespace ospf::base {

    /// 类型具有 tag() 方法返回字符串 / Type has tag() method returning string
    template<typename T>
    concept WithTag = requires(const T& t) {
        { t.tag() } -> std::convertible_to<std::string>;
    };

}  // namespace ospf::base
